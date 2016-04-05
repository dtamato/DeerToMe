// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "DeerAIController.h"
#include "DeerToMeCharacter.h"
#include "DeerAI.generated.h"


UCLASS()
class DEERTOME_API ADeerAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADeerAI();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	ADeerToMeCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly)
	ADeerAIController* DeerAIController;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	bool InRangeOfDeerCall;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Particles")
	bool HeardDeerCall;

	UPROPERTY(EditAnywhere)
	bool bIsCollected;
	
public:
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* DeerBehaviour;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBlackboardComponent* DeerBlackboard;

	UFUNCTION()
	virtual void OnBeginOverlap(AActor* OtherActor);

	UFUNCTION()
	virtual void OnEndOverlap(AActor* OtherActor);
};
