// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "DeerAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DeerAi.h"

ADeerAIController::ADeerAIController()
{
	DeerBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	DeerBehaviourTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTree"));

}

void ADeerAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ADeerAI* thisDeer = Cast<ADeerAI>(InPawn);
	if (thisDeer && thisDeer->DeerBehaviour)
	{
		DeerBlackboard->InitializeBlackboard(*thisDeer->DeerBehaviour->BlackboardAsset);
		DeerBehaviourTree->StartTree(*thisDeer->DeerBehaviour);
	}

}

