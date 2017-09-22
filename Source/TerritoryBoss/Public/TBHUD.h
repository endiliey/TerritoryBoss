// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TBHUD.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API ATBHUD : public AHUD
{
	GENERATED_BODY()

	FCanvasIcon CrosshairIcon;

	ATBHUD();

	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	void DrawCrosshair();
};
