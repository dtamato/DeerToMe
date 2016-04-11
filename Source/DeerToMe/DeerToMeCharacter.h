// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DeerToMeCharacter.generated.h"

UENUM(BlueprintType)
enum class EUI_State {

	EUI_None = 0,
	EUI_EnterRun,
	EUI_ExitRun,
	EUI_EnterCollect,
	EUI_ExitCollect,
	EUI_EnterCallOut,
	EUI_ExitCallOut,
	EUI_Starve,
	EUI_Win
};

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

public:

	ADeerToMeCharacter();

	/** Collision Sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
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

	/** Audio component */
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	class UAudioComponent* audioComponent;

	/** Played to look for deer as well as collect deer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	class USoundBase* deerCallAudio;

	/** Automatically played when deer is walking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	class USoundBase* deerWalkAudio;

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

	UFUNCTION(BlueprintPure, Category = "Power")
		bool GetGameStarted();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetGameStarted(bool GameState);

	/** Returns the current UI state*/
	UFUNCTION(BlueprintPure, Category = "UI")
		EUI_State GetCurrentUIState();

	/** Sets a new UI state*/
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetCurrentUIState(EUI_State NewState);

	/** Resets the UI state to none */
	UFUNCTION(BlueprintCallable, Category = "UI")
		void ResetCurrentUIState();

	UFUNCTION()
	void IncreaseDeersCollected();

	UFUNCTION(BlueprintPure, Category = "UI")
		uint8 GetDeersCollected();

	UFUNCTION(BlueprintPure, Category = "Camera")
		UCameraComponent* GetPlayerCamera();

	UFUNCTION(BlueprintPure, Category = "Particles")
	class ADeerAI* GetClosestDeer();

	UFUNCTION(BlueprintPure, Category = "Particles")
		bool GetPlayEffects();

	/** Toggle the players speed to a run or a walk */
	void TogglePlayerRun();

	/** fucntion to update the character's power level, PowerChange is amount to change the power by */
	void UpdateStamina(float PowerStamina);

	/** Added to refill the stamina of the deer when testing */
	void RefillStamina();

	// Function to call when the deer is seraching for another deer
	UFUNCTION(BlueprintNativeEvent)
		void CalledDeer();
	virtual void CalledDeer_Implementation();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power", Meta = (BlueprintProtected = "true"))
		bool bGameStarted;

	UPROPERTY(EditDefaultsOnly)
		float EatTimer;

	UPROPERTY(EditDefaultsOnly)
		float MaxEatTime;

	UPROPERTY(EditDefaultsOnly)
		float RunTimer;

	UPROPERTY(EditDefaultsOnly)
		float MaxRunTime;

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

	UPROPERTY()
	uint8 CollectedDeer;

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

	// Keeps track of the cuurent UI state
	EUI_State CurrentUIState;

	// For the particle animation
	UPROPERTY(VisibleAnywhere, Category = "Particles")
		float ClosestDistance;
	UPROPERTY(VisibleAnywhere, Category = "Particles")
		float EffectsTimer;
	UPROPERTY(VisibleAnywhere, Category = "Particles")
		float MaxEffectTime;
	UPROPERTY(VisibleAnywhere, Category = "Particles")
		class ADeerAI* ClosestDeer;
	UPROPERTY(VisibleAnywhere, Category = "Particles")
		bool bPlayEffects;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }

};

