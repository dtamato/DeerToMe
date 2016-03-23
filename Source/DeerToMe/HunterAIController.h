// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseAIController.h"
#include "HunterAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEERTOME_API AHunterAIController : public ABaseAIController
{
	GENERATED_BODY()
	
public:
	AHunterAIController();

public:
	//Deer Possession overriden function from base AIController
	void Possess(APawn* InPawn) override;
	
};
