// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DeerToMeCharacter.h"
#include "EndTrigger.generated.h"

UCLASS()
class DEERTOME_API AEndTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "EndTrigger")
		uint8 deerToCollect;

private:

	UPROPERTY(VisibleAnywhere, Category = "EndTrigger")
	class UBoxComponent* OverlapBox;

	class ADeerToMeGameMode* gameMode;

	// Wait to end Game Variables
	float WaitToEnd;
	float Timer;
	bool bStartWait;

};
