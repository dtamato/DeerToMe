// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class DEERTOME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Accessor function for the mesh - rreturns the mesh for the pickup
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	// Return whether or not the pickup is active
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	// Alows other classes to safely change whether or not the pickup is active
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	// Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
		void WasCollected();
	virtual void WasCollected_Implementation();

	/** Toggles the light component's visibility*/
	UFUNCTION()
		void TogglePickupLight();

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** called when something leaves the sphere component */
	UFUNCTION()
		void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	// True when the pickup can be used
	bool bIsActive;

	/** Audio component */
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	class UAudioComponent* audioComponent;

private:

	// Visual representation of object in the level 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

	/** point light component -- for when the player is near */
	UPROPERTY(VisibleAnywhere, Category = "Pickup Light")
	class UPointLightComponent* PickupLight;

	/** sphere component -- triggers light when entered */
	UPROPERTY(VisibleAnywhere, Category = "Pickup Light")
	class USphereComponent* OverlapSphere;

	/** the desired intensity for the light */
	UPROPERTY(VisibleAnywhere, Category = "Pickup Light")
	float DesiredLightIntensity;	
};
