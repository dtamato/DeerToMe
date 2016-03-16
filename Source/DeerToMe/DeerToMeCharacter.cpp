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
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DeerCall = false;
	num = 0;
	PrimaryActorTick.bCanEverTick = true;

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollctionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(100);

	// Set a base power level for the character STAMINA
	InitialStamina = 10000.f;
	CharacterStamina = InitialStamina;

	// set the dependence of the speed on the power level
	BaseSpeed = 10.0f;
	RunSpeed = 5.0f;
	WalkSpeed = 0.1f;
	SpeedFactor = WalkSpeed;
	bIsRunning = false;
	EatTimer = 0;
	MaxEatTime = 2;
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADeerToMeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("CallOut", IE_Pressed, this, &ADeerToMeCharacter::CallDeer);

	// Set this up in the editor - for picking up grass
	InputComponent->BindAction("Collect", IE_Pressed, this, &ADeerToMeCharacter::CollectPickups);
	InputComponent->BindAction("AdjustSpeed", IE_Pressed, this, &ADeerToMeCharacter::TogglePlayerRun);
	InputComponent->BindAction("RefillStamina", IE_Pressed, this, &ADeerToMeCharacter::RefillStamina);

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
		EatTimer += DeltaTime;

		if (EatTimer >= MaxEatTime) { 
			EatTimer = 0;
			bIsEating = false; 
		}
	}
}


void ADeerToMeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
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
	if ((Controller != NULL) && (Value != 0.0f) && !bIsEating)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
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
				CollectedStamina += TestGrass->GetStamina();
			}
			// Deactivate the Pickup
			TestPickup->SetActive(false);
		}
	}

	if (CollectedStamina > 0) {
		UpdateStamina(CollectedStamina);
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

void ADeerToMeCharacter::TogglePlayerRun() {
	UE_LOG(LogTemp, Warning, TEXT("Toggling Player Speed"));
	if (bIsRunning) {
		SpeedFactor = WalkSpeed;
		bIsRunning = false;
	}
	else {
		SpeedFactor = RunSpeed;
		bIsRunning = true;
	}
}

void ADeerToMeCharacter::UpdateStamina(float StanimaChange) {
	//Change power
	CharacterStamina = CharacterStamina + StanimaChange;
	// Chnage speed based on power
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * CharacterStamina;
	// Call visual effect, audio and animation
	// PowerChangeEffect();
}

void ADeerToMeCharacter::RefillStamina() {
	CharacterStamina = InitialStamina;
	if (bIsRunning) SpeedFactor = RunSpeed;
	else SpeedFactor = WalkSpeed;
}

