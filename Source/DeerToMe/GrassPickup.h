// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "GrassPickup.generated.h"

/**
 * 
 */
UCLASS()
class DEERTOME_API AGrassPickup : public APickup
{
	GENERATED_BODY()
	
	
public:

	AGrassPickup();

	void WasCollected_Implementation() override;

	float GetStamina();

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float GrassStamina;

	UPROPERTY(EditDefaultsOnly)
		float EatTimer;

	UPROPERTY(EditDefaultsOnly)
		float MaxEatTimer;

	UPROPERTY(EditDefaultsOnly)
		bool bCanEat;
};
