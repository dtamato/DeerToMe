// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "BerryPickup.h"

ABerryPickup::ABerryPickup() {

	GetMesh()->SetSimulatePhysics(true);
	
	EatTimer = 0;
	MaxEatTimer = 2;
	bCanEat = false;
}

void ABerryPickup::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	
	if (bCanEat == true) {

		EatTimer += DeltaTime;

		if (EatTimer >= MaxEatTimer) {

			OpenUpgradeMenu();
			Destroy();
		}
	}
}

void ABerryPickup::WasCollected_Implementation() {

	if (bCanEat == false) {

		Super::WasCollected_Implementation();
		bCanEat = true;
	}
}

void ABerryPickup::OpenUpgradeMenu() {

	UE_LOG(LogClass, Log, TEXT("OH HERRO."));
	if (UpgradeWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), UpgradeWidgetClass);
		if (CurrentWidget != nullptr) {
			UE_LOG(LogClass, Log, TEXT("Upgrade menUUU."));
			CurrentWidget->AddToViewport();
		}
	}
}