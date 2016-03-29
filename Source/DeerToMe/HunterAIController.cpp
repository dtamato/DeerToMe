// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "HunterAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "HunterAIController.h"

AHunterAIController::AHunterAIController()
{
	ControllerBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	ControllerBehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));

	

}

void AHunterAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AHunterAI* thisHuner = Cast<AHunterAI>(InPawn);
	if (thisHuner && thisHuner->HunterBehaviour)
	{
		ControllerBlackboard->InitializeBlackboard(*thisHuner->HunterBehaviour->BlackboardAsset);
		ControllerBehaviorTree->StartTree(*thisHuner->HunterBehaviour);
	}
}





