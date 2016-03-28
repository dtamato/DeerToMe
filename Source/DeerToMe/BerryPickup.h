// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "BerryPickup.generated.h"

/**
 * 
 */
UCLASS()
class DEERTOME_API ABerryPickup : public APickup
{
	GENERATED_BODY()

public:

	ABerryPickup();

	void WasCollected_Implementation() override;

	void OpenUpgradeMenu();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly)
		float EatTimer;

	UPROPERTY(EditDefaultsOnly)
		float MaxEatTimer;

	UPROPERTY(EditDefaultsOnly)
		bool bCanEat;

	/** widget class to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> UpgradeWidgetClass;

	UPROPERTY()
		class UUserWidget* CurrentWidget;
};
