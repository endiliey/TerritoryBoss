// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBPlayerCameraManager.h"
#include "TBCharacter.h"

ATBPlayerCameraManager::ATBPlayerCameraManager()
{
	NormalFOV = 90.0f;
	TargetingFOV = 65.0f;

	ViewPitchMin = -80.0f;
	ViewPitchMax = 87.0f;
	bAlwaysApplyModifiers = true;
}


void ATBPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	ATBCharacter* MyPawn = PCOwner ? Cast<ATBCharacter>(PCOwner->GetPawn()) : NULL;
	if (MyPawn)
	{
		const float TargetFOV = MyPawn->IsTargeting() ? TargetingFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
		SetFOV(DefaultFOV);
	}
	Super::UpdateCamera(DeltaTime);
}


