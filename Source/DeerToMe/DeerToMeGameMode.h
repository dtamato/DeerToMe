// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "DeerToMeGameMode.generated.h"

UCLASS(minimalapi)
class ADeerToMeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADeerToMeGameMode();

	virtual void Tick(float DeltaTime) override;

	/**returns power needed to win - needed for HUD*/
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetMaxStamina() const;

	virtual void BeginPlay() override;

	void RemoveUI();

protected:
	// rate at which the character loses power
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power")
		float DecayRate;

	/**Power needed to win game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float MaxStamina;

	/** widget class to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** the instance of the HUD*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power")
	class UUserWidget* CurrentWidget;

	class EndTrigger* endTrigger;

	bool bUIDisplayed;

};



