// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DeerToMe.h"
#include "Engine.h"
#include "DeerToMeCharacter.h"
#include "Pickup.h"
#include "GrassPickup.h"

//////////////////////////////////////////////////////////////////////////
// ADeerToMeCharacter

ADeerToMeCharacter::ADeerToMeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CallOutDistance = CreateDefaultSubobject<USphereComponent>(TEXT("CallOut"));
	CallOutDistance->AttachTo(RootComponent);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the audio component
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	//audioComponent->SetSound(audioFile);
	audioComponent->AttachParent = RootComponent;

	DeerCall = false;
	num = 0;
	PrimaryActorTick.bCanEverTick = true;

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollctionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(100);

	// Set a base power level for the character STAMINA
	InitialStamina = 1000.0f;
	CharacterStamina = InitialStamina;

	// set the dependence of the speed on the power level
	BaseSpeed = 100.0f;
	SpeedFactor = 10;
	RunTimer = 0;
	MaxRunTime = 4;
	RunBoost = 3000;
	CollectedDeer = 0;

	JumpTimer = 0;
	JumpWaitTime = 1;
	
	EatTimer = 0;
	MaxEatTime = 2;

	bIsRunning = false;
	bIsJumping = false;
	bCheckJump = false;
	bCheckRun = false;
	bIsStarved = false;
	bGameStarted = false;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADeerToMeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ADeerToMeCharacter::StartDeerJump);
	// InputComponent->BindAction("Jump", IE_Released, this, &ADeerToMeCharacter::StopDeerJump);

	InputComponent->BindAction("CallOut", IE_Pressed, this, &ADeerToMeCharacter::CallDeer);

	// Set this up in the editor - for picking up grass
	InputComponent->BindAction("Collect", IE_Pressed, this, &ADeerToMeCharacter::CollectPickups);
	InputComponent->BindAction("AdjustSpeed", IE_Pressed, this, &ADeerToMeCharacter::TogglePlayerRun);
	InputComponent->BindAction("RefillStamina", IE_Pressed, this, &ADeerToMeCharacter::RefillStamina);
	InputComponent->BindAction("Look", IE_Pressed, this, &ADeerToMeCharacter::CalledDeer);

	InputComponent->BindAxis("MoveForward", this, &ADeerToMeCharacter::MoveForward);
	// InputComponent->BindAxis("MoveRight", this, &ADeerToMeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ADeerToMeCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ADeerToMeCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ADeerToMeCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ADeerToMeCharacter::TouchStopped);
}

void ADeerToMeCharacter::CallDeer()
{
	DeerCall = true;
	//CallOutDistance->Activate(true);
	FString Message = FString::Printf(TEXT("Hit Box On"));
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Black, Message);

	audioComponent->SetSound(deerCallAudio);
	audioComponent->Play();
}

void ADeerToMeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DeerCall == true)
	{
		                
		//Need to make the hit boxes work for overlap with the AI
		if (num <= 1.0f)
		{
			num += 1.0f * DeltaTime;
		}
		else
		{
			DeerCall = false;
			num = 0;
			CallOutDistance->Activate(false);
			FString Message = FString::Printf(TEXT("Hit box off"));
			GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, Message);
		}
	}

	if (bIsEating == true) {
		EatGrass(DeltaTime);
	}

	if (bIsJumping == true) {
		RunDeerJump(DeltaTime);
		CheckJump(DeltaTime);
	}

	if (bIsRunning) {
		RunTimer += DeltaTime;
		if (RunTimer >= MaxRunTime) {
			bIsRunning = false;
			RunTimer = 0;
		}
	}

	if (CharacterStamina <= 0 && bIsStarved == false) { 
		SetCurrentUIState(EUI_State::EUI_Starve); 
		bIsStarved = true;
	}

}

void ADeerToMeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		bIsJumping = true;
		Jump();
	}
}

void ADeerToMeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ADeerToMeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADeerToMeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADeerToMeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bIsEating && !bIsStarved)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (bIsJumping) { Value *= 0.5; }

		AddMovementInput(Direction, Value);

		// Play walking audio
		if (audioComponent->IsPlaying() == false) {
			audioComponent->SetSound(deerWalkAudio);
			audioComponent->Play();
		}
	}
	else if (Value == 0.0f) {

		if (audioComponent->IsPlaying()) {
			audioComponent->Stop();
		}
	}
}

void ADeerToMeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !bIsEating)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADeerToMeCharacter::CollectPickups() {
	if (CharacterStamina < InitialStamina - 10) {
		// Get all overlapping  actors and store then in an array
		TArray<AActor*> CollectedActors;
		CollectionSphere->GetOverlappingActors(CollectedActors);

		// Keep track of collected poewer
		float CollectedStamina = 0;

		// this function loops though all of the pickups in the volume and refills the character power accordingly for each actor we collected 
		for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected) {
			// Cast the actor to APikcup
			APickup* TestPickup = Cast<APickup>(CollectedActors[iCollected]);
			// If the cast is successfula dn the pickup is valid and active
			if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive()) {
				// Set eating boolean to true
				bIsEating = true;
				// Then call the pickups WasCollected function
				TestPickup->WasCollected();
				// Check to see if pickup is battery
				AGrassPickup* TestGrass = Cast<AGrassPickup>(TestPickup);
				if (TestGrass) {
					// Increase the collected power
					UE_LOG(LogTemp, Warning, TEXT("Eating"));
					CollectedStamina += TestGrass->GetStamina();
					SetCurrentUIState(EUI_State::EUI_ExitCollect);
				}
				// Deactivate the Pickup
				TestPickup->SetActive(false);
			}
		}

		if (CollectedStamina > 0) {
			UpdateStamina(CollectedStamina);
		}
	}
}

float ADeerToMeCharacter::GetInitilaStamina() {
	return InitialStamina;
}

float ADeerToMeCharacter::GetCurrentStamina() {
	return CharacterStamina;
}

bool ADeerToMeCharacter::GetIsRunning() {
	return bIsRunning;
}

bool ADeerToMeCharacter::GetIsEating() {
	return bIsEating;
}

bool ADeerToMeCharacter::GetIsJumping() {
	return bIsJumping;
}

bool ADeerToMeCharacter::GetIsStarving() {
	return bIsStarved;
}

bool ADeerToMeCharacter::GetGameStarted() {
	return bGameStarted;
}

void ADeerToMeCharacter::SetGameStarted(bool GameState) {
	bGameStarted = GameState;
}

EUI_State ADeerToMeCharacter::GetCurrentUIState() {
	return CurrentUIState;
}

void ADeerToMeCharacter::SetCurrentUIState(EUI_State NewState) {
	UE_LOG(LogTemp, Warning, TEXT("Setting player UI State"));
	CurrentUIState = NewState;
}

void ADeerToMeCharacter::ResetCurrentUIState() {
	UE_LOG(LogTemp, Warning, TEXT("resetting player UI State"));
	CurrentUIState = EUI_State::EUI_None;
}

void ADeerToMeCharacter::IncreaseDeersCollected() {
	CollectedDeer++;
}

uint8 ADeerToMeCharacter::GetDeersCollected() {
	return CollectedDeer;
}

UCameraComponent* ADeerToMeCharacter::GetPlayerCamera() {
	return FollowCamera;
}

void ADeerToMeCharacter::TogglePlayerRun() {
	UE_LOG(LogTemp, Warning, TEXT("Toggling Player Speed"));
	// Check to see if the player still has half their stamina
	if (CharacterStamina >= (InitialStamina * 0.3f) && !bIsRunning) {
		// if they do allow them to run -- countdown in tick -- add in a timed variable
		bIsRunning = true;
		// Decrease their stamina by a quarter
		CharacterStamina -= (InitialStamina * 0.25f);
	}	
}

void ADeerToMeCharacter::UpdateStamina(float StanimaChange) {
	//Change power
	CharacterStamina = CharacterStamina + StanimaChange;

	if (CharacterStamina > InitialStamina) { CharacterStamina = InitialStamina; }

	// Chnage speed based on power
	if (!bIsRunning) {
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * (CharacterStamina * 0.1f);
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor + RunBoost;
	}

	// UE_LOG(LogTemp, Warning, TEXT("Player Max Speed %f"), GetCharacterMovement()->MaxWalkSpeed);
	// Call visual effect, audio and animation
}

void ADeerToMeCharacter::RefillStamina() {
	CharacterStamina = InitialStamina;
	SpeedFactor = BaseSpeed;
}

void ADeerToMeCharacter::StartDeerJump() {
	if (GetCharacterMovement()->Velocity.Size() <= 5000) {
		bIsJumping = true;
	}
}

void ADeerToMeCharacter::StopDeerJump() {
	bCheckJump = false;
	bIsJumping = false;
	StopJumping();
}

void ADeerToMeCharacter::RunDeerJump(float DeltaTime) {
	
	// UE_LOG(LogClass, Log, TEXT("increasing wait time"));
	JumpTimer += DeltaTime;
	if (JumpTimer >= JumpWaitTime) {
		Jump();
		bCheckJump = true;
	}
}

void ADeerToMeCharacter::EatGrass(float DeltaTime) {
	EatTimer += DeltaTime;

	if (EatTimer >= MaxEatTime) {
		EatTimer = 0;
		bIsEating = false;
	}
}

void ADeerToMeCharacter::CheckJump(float DeltaTime) {
	if (bCheckJump) {
		JumpTimer += DeltaTime;
		if (JumpTimer >= JumpWaitTime) {
			if (abs((long)GetCharacterMovement()->Velocity.Z) == 0) {
				JumpTimer = 0;
				StopDeerJump();
			}
		}
	}
}

void ADeerToMeCharacter::CalledDeer_Implementation() {
	//Load a debug message
	UE_LOG(LogClass, Log, TEXT("You have called %s"));
}