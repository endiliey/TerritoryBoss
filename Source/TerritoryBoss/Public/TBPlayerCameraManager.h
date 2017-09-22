// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "TBPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API ATBPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	ATBPlayerCameraManager();

	// Update the FOV
	virtual void UpdateCamera(float DeltaTime) override;
	
public:

	// default, hip fire FOV
	float NormalFOV;

	// aiming down sight / zoomed FOV
	float TargetingFOV;
};
