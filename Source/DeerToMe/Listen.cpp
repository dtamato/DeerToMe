// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "DeerAI.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Listen.h"


EBTNodeResult::Type UListen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	FBlackboard::FKey PlayerKey = BlackBoard->GetKeyID(Player);
	
	if (GetWorld() == NULL)
	{
		return EBTNodeResult::Failed;
	}

	for (TActorIterator<ADeerAI> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ADeerAI* NewDeer = *ActorItr;
		if (NewDeer && NewDeer->HeardDeerCall == true)
		{	
			//This is for the setting of the AI to find the player
			ADeerToMeCharacter* DeerPlayer = NewDeer->PlayerCharacter;

			bool DeerCallHeard = NewDeer->InRangeOfDeerCall;
			if (NewDeer->InRangeOfDeerCall == true)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerKey, DeerPlayer);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
