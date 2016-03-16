// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "DeerToMeCharacter.h" 
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Listen.generated.h"

/**
 * 
 */
UCLASS()
class DEERTOME_API UListen : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FName Player = "Player";
	FName HeardDeer = "HearDeer";
};
