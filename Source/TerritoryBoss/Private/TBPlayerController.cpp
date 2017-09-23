// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBPlayerController.h"
#include "TBPlayerCameraManager.h"

ATBPlayerController::ATBPlayerController()
{
	PlayerCameraManagerClass = ATBPlayerCameraManager::StaticClass();
}