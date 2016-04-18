// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "HunterDistanceToDeerCheckTask.generated.h"

/**
 * 
 */
UCLASS()
class DEERTOME_API UHunterDistanceToDeerCheckTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool InLineOfSight(AHunterAI* Target1, ADeerToMeCharacter* Target2);

	FName Player = "Player";

private:

	bool bShotPlayer = false;
	
};
