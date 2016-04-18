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
	
	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(PlayerKey))
	{
		return EBTNodeResult::Succeeded;
	}

	ADeerAIController* NewDeer = Cast<ADeerAIController>(OwnerComp.GetAIOwner());
	if (NewDeer)
	{
		ADeerAI* DeerController = Cast<ADeerAI>(NewDeer->GetCharacter());

		if (DeerController && DeerController->HeardDeerCall)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerKey, DeerController->PlayerCharacter);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
