// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DeerToMe.h"
#include "DeerToMeGameMode.h"
#include "DeerToMeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

ADeerToMeGameMode::ADeerToMeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/DeerToMePlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Base deacy Rate
	MinDecayRate = 0.05f;
	MaxDecayRate = 0.1f;
	DecayRate = MinDecayRate;
	MaxStamina = 100;
}

void ADeerToMeGameMode::BeginPlay() {
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}
}

void ADeerToMeGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// access our current character
	ADeerToMeCharacter* MyCharacter = Cast<ADeerToMeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter) {

		FVector CharacterVelocity = MyCharacter->GetVelocity();

		if (CharacterVelocity.Size() > 0) {

			// if our power is greater than needed to win, set the gaem state to won
			MaxStamina = (MyCharacter->GetInitilaStamina());

			// Check to see if the player is running or walking and adjust the decay accordingly
			if (MyCharacter->GetIsRunning()) { DecayRate = MaxDecayRate; }
			else { DecayRate = MinDecayRate; }

			// If the character still has power decrease it gradually using DecayRate
			if (MyCharacter->GetCurrentStamina() > 0.3f) { MyCharacter->UpdateStamina(-DeltaTime * DecayRate * (MyCharacter->GetInitilaStamina())); }
		}
	}
}

float ADeerToMeGameMode::GetMaxStamina() const {
	return MaxStamina;
}