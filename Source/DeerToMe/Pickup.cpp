// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "Pickup.h"
#include "DeerToMeCharacter.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// All pickups start active
	bIsActive = true;

	// Create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;

	// Create the Point Light
	PickupLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Pickup Light"));
	PickupLight->Intensity = DesiredLightIntensity;
	PickupLight->bVisible = false;
	PickupLight->AttachParent = RootComponent;

	// Create the Overlap Sphere
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap Sphere"));
	OverlapSphere->InitSphereRadius(250.f);
	OverlapSphere->AttachParent = RootComponent;

	// Designate the OnOverlap function as a delegate called when an actor overlaps or leaves the sphere component
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &APickup::OnOverlapEnd);

	// The brightness of the light when sphere is entered
	DesiredLightIntensity = 3000.0f;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Returns active state
bool APickup::IsActive() {
	return bIsActive;
}

// Changes Active state
void APickup::SetActive(bool NewPickupState) {
	bIsActive = NewPickupState;
}

void APickup::WasCollected_Implementation() {
	//Loag a debug message
	FString PickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected %s"), *PickupDebugString);
}

void APickup::TogglePickupLight() {
	PickupLight->ToggleVisibility();
}

void APickup::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (OtherActor && OtherActor != this && OtherComp) {
		
		ADeerToMeCharacter* DeerCharacter = Cast<ADeerToMeCharacter>(OtherActor);
		
		if (DeerCharacter && PickupLight->bVisible == false) {
			UE_LOG(LogClass, Log, TEXT("On Overlap Begin Called."));
			TogglePickupLight();
		}
	}
}

void APickup::OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor && OtherActor != this && OtherComp) {
		
		ADeerToMeCharacter* DeerCharacter = Cast<ADeerToMeCharacter>(OtherActor);

		if (DeerCharacter && PickupLight->bVisible == true) {
			UE_LOG(LogClass, Log, TEXT("On Overlap End Called."));
			TogglePickupLight();
		}
	}
}