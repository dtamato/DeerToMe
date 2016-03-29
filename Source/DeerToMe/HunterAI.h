// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
//#include "Runtime/Engine/Classes/Components/AudioComponent.h"
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

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGunSound();

public:
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* HunterBehaviour;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBlackboardComponent* HunterBlackboard;

	UPROPERTY(EditDefaultsOnly)
	ADeerToMeCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	UAudioComponent* GunShotAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* GunSound;
};
