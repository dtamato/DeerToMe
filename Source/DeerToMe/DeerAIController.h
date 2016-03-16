// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIController.h"
#include "DeerAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEERTOME_API ADeerAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	ADeerAIController();

public:
	//Deer Possession overriden function from base AIController
	void Possess(APawn* InPawn) override;
};
