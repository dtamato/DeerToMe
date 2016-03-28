// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "GrassPickup.h"

AGrassPickup::AGrassPickup() {
	GetMesh()->SetSimulatePhysics(true);

	// The amount of stamina the grass will provide the deer
	GrassStamina = 1000.0f;
	EatTimer = 0;
	MaxEatTimer = 2;
	bCanEat = false;
}

// Called every frame
void AGrassPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanEat == true) {
		EatTimer += DeltaTime;
		if (EatTimer >= MaxEatTimer) {
			Destroy();
		}
	}
}

void AGrassPickup::WasCollected_Implementation() {
	if (bCanEat == false) {
		// Use the behaviour derrived from the base pickup
		Super::WasCollected_Implementation();
		// Put in a delay before destroying the pickup
		bCanEat = true;
	}
}

float AGrassPickup::GetStamina() {
	return GrassStamina;
}
