// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DeerToMeCharacter.h"
#include "HunterAI.generated.h"

UCLASS()
class DEERTOME_API AHunterAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHunterAI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* HunterBehaviour;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBlackboardComponent* HunterBlackboard;

	UPROPERTY(EditAnywhere, Category = Behavior)
	float MaxDistanceFromPlayer;
	
	UPROPERTY(EditAnywhere, Category = Behavior)
	float DistanceFromPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float VingetteIntensity;

	UPROPERTY(EditDefaultsOnly)
	ADeerToMeCharacter* PlayerCharacter;
	
	// Accessor for InitialPower
	UFUNCTION(BlueprintPure, Category = "Camera")
	float GetVingetteIntensity();
};
