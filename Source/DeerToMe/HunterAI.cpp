// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "Engine.h"
#include "EngineUtils.h"
#include "HunterAI.h"


// Sets default values
AHunterAI::AHunterAI()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxDistanceFromPlayer = 10000;
	bNotifiedUI = false;
}

// Called when the game starts or when spawned
void AHunterAI::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ADeerToMeCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ADeerToMeCharacter* TestCharacter = Cast<ADeerToMeCharacter>(*ActorItr);
		if (TestCharacter)
		{
			PlayerCharacter = TestCharacter;
		}
	}
}

void AHunterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceFromPlayer = GetDistanceTo(PlayerCharacter);
	
	if (DistanceFromPlayer <= MaxDistanceFromPlayer) {
		if (PlayerCharacter->GetCurrentUIState() == EUI_State::EUI_None && bNotifiedUI == false) {
			PlayerCharacter->SetCurrentUIState(EUI_State::EUI_EnterRun);
			bNotifiedUI = true;
		}
		
		ScreenColorIntensity = DistanceFromPlayer / MaxDistanceFromPlayer;
		VingetteIntensity = 1 - ScreenColorIntensity;
	}
	else {
		if (bNotifiedUI == true) {
			PlayerCharacter->SetCurrentUIState(EUI_State::EUI_ExitRun);
			bNotifiedUI = false;
		}
	}
}

// Called to bind functionality to input
void AHunterAI::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

float AHunterAI::GetVingetteIntensity() {
	return VingetteIntensity;
}