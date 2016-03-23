// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "DeerAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DeerAi.h"

ADeerAIController::ADeerAIController()
{
	ControllerBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	ControllerBehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));

}

void ADeerAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ADeerAI* thisDeer = Cast<ADeerAI>(InPawn);
	if (thisDeer && thisDeer->DeerBehaviour)
	{
		ControllerBlackboard->InitializeBlackboard(*thisDeer->DeerBehaviour->BlackboardAsset);
		ControllerBehaviorTree->StartTree(*thisDeer->DeerBehaviour);
	}

}

