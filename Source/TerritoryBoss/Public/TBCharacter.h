// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "TBCharacter.generated.h"

UCLASS(config=Game)
class ATBCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning to handle camera distance to the character mesh */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	/** Primary camera of the player / Follow camera */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

public:
	ATBCharacter();

	virtual void PostInitializeComponents() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

protected:

	// Bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/************************************************************************/
	/* Movement                                                             */
	/************************************************************************/

	void MoveForward(float Value);

	void MoveRight(float Value);

	void OnCrouchToggle();

	void OnStartJump();

	void OnStopJump();

	void OnStartSprinting();

	void OnStopSprinting();

	/* Character wants to run, checked during Tick to see if allowed */
	UPROPERTY(Transient, Replicated)
	bool bWantsToRun;

	/* Is character currently performing a jump action. Resets on landed.  */
	UPROPERTY(Transient, Replicated)
	bool bIsJumping;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;

	void SetIsJumping(bool NewJumping);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetIsJumping(bool NewJumping);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	/* Client/local call to update sprint state  */
	void SetSprinting(bool NewSprinting);

	/* Server side call to update actual sprint state */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(bool NewSprinting);

	UFUNCTION(BlueprintCallable, Category = Movement)
	bool IsSprinting() const;

	float GetSprintingSpeedModifier() const;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintingSpeedModifier;

	/************************************************************************/
	/* Object Interaction                                                   */
	/************************************************************************/

	///* Use the usable actor currently in focus, if any */
	//virtual void Use();

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerUse();

	//class ATBUsableActor* GetUsableInView();

	///*Max distance to use/focus on actors. */
	//UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	//float MaxUseDistance;

	///* True only in first frame when focused on a new usable actor. */
	//bool bHasNewFocus;

	//class ATBUsableActor* FocusedUsableActor;

	/************************************************************************/
	/* Targeting                                                            */
	/************************************************************************/
	
	void OnStartTargeting();

	void OnEndTargeting();

	void SetTargeting(bool NewTargeting);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetTargeting(bool NewTargeting);

	/* Is player aiming down sights */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	bool IsTargeting() const;

	float GetTargetingSpeedModifier() const;

	/* Retrieve Pitch/Yaw from current camera */
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	FRotator GetAimOffsets() const;

	UPROPERTY(Transient, Replicated)
	bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingSpeedModifier;

	/************************************************************************/
	/* Hitpoints                                                  */
	/************************************************************************/

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	void ConsumeFood(float AmountRestored);

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	bool IsAlive() const;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition", Replicated)
	float Health;

	/************************************************************************/
	/* Damage, Hit & Death                                                  */
	/************************************************************************/

	/* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

