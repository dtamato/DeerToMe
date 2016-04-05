// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "Engine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "DeerAI.h"


// Sets default values
ADeerAI::ADeerAI()
{
	PrimaryActorTick.bCanEverTick = true;	
	OnActorBeginOverlap.AddDynamic(this, &ADeerAI::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ADeerAI::OnEndOverlap);
	HeardDeerCall = false;
	InRangeOfDeerCall = false;
	bIsCollected = false;
}

void ADeerAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InRangeOfDeerCall == true)
	{
		if (PlayerCharacter->DeerCall == true)
		{
			HeardDeerCall = true;
		}
	}

	if (HeardDeerCall == true && bIsCollected == false) {
		PlayerCharacter->IncreaseDeersCollected();
		bIsCollected = true;
	}
}

// Called when the game starts or when spawned
void ADeerAI::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ADeerToMeCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		PlayerCharacter = *ActorItr;
	}
}

void ADeerAI::OnBeginOverlap(AActor* OtherActor)
{
	PlayerCharacter = Cast<ADeerToMeCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		if(PlayerCharacter->GetCurrentUIState() == EUI_State::EUI_None)
			PlayerCharacter->SetCurrentUIState(EUI_State::EUI_EnterCallOut);
		InRangeOfDeerCall = true;
	}
}

void ADeerAI::OnEndOverlap(AActor* OtherActor)
{
	PlayerCharacter = Cast<ADeerToMeCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetCurrentUIState() == EUI_State::EUI_None)
		PlayerCharacter->SetCurrentUIState(EUI_State::EUI_ExitCallOut);
		InRangeOfDeerCall = false;
	}
}


