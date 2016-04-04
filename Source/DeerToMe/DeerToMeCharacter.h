// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DeerToMeCharacter.generated.h"

UCLASS(config=Game)
class ADeerToMeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CallOutDistance;

	/** Collision Sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

public:
	ADeerToMeCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/* Delete theses later their just for testing */
	UPROPERTY(EditDefaultsOnly)
	bool DeerCall;

	UPROPERTY(EditDefaultsOnly)
	float num;

	UPROPERTY(EditDefaultsOnly)
	float JumpTimer;

	UPROPERTY(EditDefaultsOnly)
	float JumpWaitTime;

	UFUNCTION()
	void CallDeer();

	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly)
	FName Player;

	UPROPERTY()
	uint8 BlackboardKeyID_Player;

	// Start Added 
	// Accessor for InitialPower
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetInitilaStamina();
	
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetCurrentStamina();

	UFUNCTION(BlueprintPure, Category = "Power")
		bool GetIsRunning();

	UFUNCTION(BlueprintPure, Category = "Power")
		bool GetIsEating();

	UFUNCTION(BlueprintPure, Category = "Power")
		bool GetIsJumping();

	UFUNCTION(BlueprintPure, Category = "Power")
		bool GetIsStarving();

	UFUNCTION(BlueprintPure, Category = "Camera")
		UCameraComponent* GetPlayerCamera();

	/** Toggle the players speed to a run or a walk */
	void TogglePlayerRun();

	/** fucntion to update the character's power level, PowerChange is amount to change the power by */
	void UpdateStamina(float PowerStamina);

	/** Added to refill the stamina of the deer when testing */
	void RefillStamina();

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	// Called when we press a key to collect any pickups inside the collection sphere
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void CollectPickups();

	// the starting power level of the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float InitialStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float RunStaminaDecay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float WalkStaminaDecay;

	/** Multiplier for character speed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float SpeedFactor;

	/** Speed when power level is 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float BaseSpeed;

	/** Current Speed of character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		bool bIsEating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		bool bIsJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power", Meta = (BlueprintProtected = "true"))
		bool bIsStarved;

	UPROPERTY(EditDefaultsOnly)
		float EatTimer;

	UPROPERTY(EditDefaultsOnly)
		float MaxEatTime;

	UPROPERTY(EditDefaultsOnly)
		float RunTimer;

	UPROPERTY(EditDefaultsOnly)
		float MaxRunTime;

	// UFUNCTION(BlueprintImplementableEvent, Category = "Power")
	// 	void PowerChangeEffect();

private:
	// Current poweer level of the character
	UPROPERTY(VisibleAnywhere, Category = "Power")
	float CharacterStamina;

	UPROPERTY(VisibleAnywhere, Category = "Power")
	float RunBoost;

	UPROPERTY()
	bool bCheckJump;

	UPROPERTY()
	bool bCheckRun;

	UFUNCTION()
	void StartDeerJump();
	
	UFUNCTION()
	void StopDeerJump();

	UFUNCTION()
	void RunDeerJump(float DeltaTime);

	UFUNCTION()
	void EatGrass(float DeltaTime);

	UFUNCTION()
	void CheckJump(float DeltaTime);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }

};

