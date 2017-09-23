// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TerritoryBoss.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "TBCharacter.h"
#include "TBUsableActor.h"
#include "TBCharacterMovementComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATBCharacter

ATBCharacter::ATBCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTBCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	// Configure character movement
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->JumpZVelocity = 600.f;
	MoveComp->GravityScale = 1.5f;
	MoveComp->AirControl = 0.2f;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->MaxWalkSpeedCrouched = 200;

	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SocketOffset = FVector(0, 35, 0);
	CameraBoom->TargetOffset = FVector(0, 0, 55);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetupAttachment(RootComponent);
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation

	MaxUseDistance = 800;
	bHasNewFocus = true;
	TargetingSpeedModifier = 0.5f;
	SprintingSpeedModifier = 2.5f;

	Health = 100;
}

void ATBCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Value is already updated locally, skip in replication step
	DOREPLIFETIME_CONDITION(ATBCharacter, bWantsToRun, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ATBCharacter, bIsTargeting, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ATBCharacter, bIsJumping, COND_SkipOwner);

	// Replicate to every client, no special condition required
	DOREPLIFETIME(ATBCharacter, Health);
}

void ATBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATBCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATBCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATBCharacter::OnStopJump);
	PlayerInputComponent->BindAction("SprintHold", IE_Pressed, this, &ATBCharacter::OnStartSprinting);
	PlayerInputComponent->BindAction("SprintHold", IE_Released, this, &ATBCharacter::OnStopSprinting);
	PlayerInputComponent->BindAction("CrouchToggle", IE_Pressed, this, &ATBCharacter::OnCrouchToggle);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ATBCharacter::Use);
	PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &ATBCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Targeting", IE_Released, this, &ATBCharacter::OnEndTargeting);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATBCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATBCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ATBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bWantsToRun && !IsSprinting())
	{
		SetSprinting(true);

		if (Controller && Controller->IsLocalController())
		{
			ATBUsableActor* Usable = GetUsableInView();

			// End Focus
			if (FocusedUsableActor != Usable)
			{
				if (FocusedUsableActor)
				{
					FocusedUsableActor->OnEndFocus();
				}

				bHasNewFocus = true;
			}

			// Assign new Focus
			FocusedUsableActor = Usable;

			// Start Focus.
			if (Usable)
			{
				if (bHasNewFocus)
				{
					Usable->OnBeginFocus();
					bHasNewFocus = false;
				}
			}
		}
	}
}

void ATBCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATBCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATBCharacter::OnCrouchToggle()
{	
	if (IsSprinting())
	{
		SetSprinting(false);
	}

	// If we are crouching then CanCrouch will return false. If we cannot crouch then calling Crouch() wont do anything
	if (CanCrouch())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ATBCharacter::OnStartJump()
{
	bPressedJump = true;

	SetIsJumping(true);
}


void ATBCharacter::OnStopJump()
{
	bPressedJump = false;
}

void ATBCharacter::OnStartSprinting()
{
	SetSprinting(true);
}


void ATBCharacter::OnStopSprinting()
{
	SetSprinting(false);
}

bool ATBCharacter::IsInitiatedJump() const
{
	return bIsJumping;
}

void ATBCharacter::SetIsJumping(bool NewJumping)
{
	// Go to standing pose if trying to jump while crouched
	if (bIsCrouched && NewJumping)
	{
		UnCrouch();
	}
	else if (NewJumping != bIsJumping)
	{
		bIsJumping = NewJumping;

		if (bIsJumping)
		{
			/* Perform the built-in Jump on the character */
			Jump();
		}
	}

	if (Role < ROLE_Authority)
	{
		ServerSetIsJumping(NewJumping);
	}
}

void ATBCharacter::ServerSetIsJumping_Implementation(bool NewJumping)
{
	SetIsJumping(NewJumping);
}

bool ATBCharacter::ServerSetIsJumping_Validate(bool NewJumping)
{
	return true;
}

void ATBCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	/* Check if we are no longer falling/jumping */
	if (PrevMovementMode == EMovementMode::MOVE_Falling &&
		GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		SetIsJumping(false);
	}
}

void ATBCharacter::SetSprinting(bool NewSprinting)
{
	bWantsToRun = NewSprinting;

	if (bIsCrouched)
		UnCrouch();

	// TODO: Stop weapon fire

	if (Role < ROLE_Authority)
	{
		ServerSetSprinting(NewSprinting);
	}
}

void ATBCharacter::ServerSetSprinting_Implementation(bool NewSprinting)
{
	SetSprinting(NewSprinting);
}

bool ATBCharacter::ServerSetSprinting_Validate(bool NewSprinting)
{
	return true;
}

bool ATBCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
		return false;

	return bWantsToRun && !IsTargeting() && !GetVelocity().IsZero()
		// Don't allow sprint while strafing sideways or standing still (1.0 is straight forward, -1.0 is backward while near 0 is sideways or standing still)
		&& (GetVelocity().GetSafeNormal2D() | GetActorRotation().Vector()) > 0.8; // Changing this value to 0.1 allows for diagonal sprinting. (holding W+A or W+D keys)
}

float ATBCharacter::GetSprintingSpeedModifier() const
{
	return SprintingSpeedModifier;
}

void ATBCharacter::Use()
{
	// Only allow on server. If called on client push this request to the server
	if (Role == ROLE_Authority)
	{
		ATBUsableActor* Usable = GetUsableInView();
		if (Usable)
		{
			Usable->OnUsed(this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ATBCharacter::ServerUse_Implementation()
{
	Use();
}

bool ATBCharacter::ServerUse_Validate()
{
	return true;
}

/*
Performs ray-trace to find closest looked-at UsableActor.
*/
ATBUsableActor* ATBCharacter::GetUsableInView()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == NULL)
		return NULL;

	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(TEXT("TraceUsableActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	return Cast<ATBUsableActor>(Hit.GetActor());
}

void ATBCharacter::OnStartTargeting()
{
	SetTargeting(true);
}

void ATBCharacter::OnEndTargeting()
{
	SetTargeting(false);
}

void ATBCharacter::SetTargeting(bool NewTargeting)
{
	bIsTargeting = NewTargeting;

	if (Role < ROLE_Authority)
	{
		ServerSetTargeting(NewTargeting);
	}
}

void ATBCharacter::ServerSetTargeting_Implementation(bool NewTargeting)
{
	SetTargeting(NewTargeting);
}


bool ATBCharacter::ServerSetTargeting_Validate(bool NewTargeting)
{
	return true;
}

bool ATBCharacter::IsTargeting() const
{
	return bIsTargeting;
}

float ATBCharacter::GetTargetingSpeedModifier() const
{
	return TargetingSpeedModifier;
}

FRotator ATBCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

float ATBCharacter::GetHealth() const
{
	return Health;
}

float ATBCharacter::GetMaxHealth() const
{
	// Retrieve the default value of the health property that is assigned on instantiation.
	return GetClass()->GetDefaultObject<ATBCharacter>()->Health;
}

void ATBCharacter::ConsumeFood(float AmountRestored)
{
	// Restore Hitpoints
	Health = FMath::Clamp(Health + AmountRestored, 0.0f, GetMaxHealth());
}

bool ATBCharacter::IsAlive() const
{
	return Health > 0;
}

float ATBCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0)
		{
			// TODO: Handle death
			//Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			// TODO: Play hit
			//PlayHit(ActualDamage, DamageEvent, EventInstigator->GetPawn(), DamageCauser, false);
		}
	}

	return ActualDamage;
}