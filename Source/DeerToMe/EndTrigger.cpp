// Fill out your copyright notice in the Description page of Project Settings.

#include "DeerToMe.h"
#include "EndTrigger.h"


// Sets default values
AEndTrigger::AEndTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Overlap Sphere
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Box"));
	OverlapBox->InitBoxExtent(FVector(3, 3, 3));
	OverlapBox->AttachParent = RootComponent;

	// Designate the OnOverlap function as a delegate called when an actor overlaps or leaves the sphere component
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AEndTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AEndTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEndTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AEndTrigger::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor && OtherActor != this && OtherComp) {

		ADeerToMeCharacter* DeerCharacter = Cast<ADeerToMeCharacter>(OtherActor);

		if (DeerCharacter && DeerCharacter->GetDeersCollected() == deerToCollect) {
			
			UE_LOG(LogClass, Warning, TEXT("GOOD JORB MAN"));
			//UGameplayStatics::OpenLevel()
			GetWorld()->ServerTravel(FString("/Game/Maps/Dave"));
		}
		else {

			UE_LOG(LogClass, Warning, TEXT("You have collected %i deer. You need %i."), DeerCharacter->GetDeersCollected(), deerToCollect);
		}
	}
}

