// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */

UCLASS()
class DEERTOME_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		UBlackboardComponent* DeerBlackboard;

	/* Cached BT component */
	UPROPERTY(EditDefaultsOnly)
		UBehaviorTreeComponent* DeerBehaviourTree;
public:

	//Possess a character
	virtual void Possess(APawn* InPawn) override;

	//Follow an object 
	virtual void Seek(APawn* Target);

	//Run away from target
	virtual void Flee(APawn* Target);

public:
	/** Returns BlackboardComp subobject **/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return DeerBlackboard; }
	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return DeerBehaviourTree; }
	
};
