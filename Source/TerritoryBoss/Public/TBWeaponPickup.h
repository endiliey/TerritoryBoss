// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBPickupActor.h"
#include "TBWeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API ATBWeaponPickup : public ATBPickupActor
{
	GENERATED_BODY()

public:

	ATBWeaponPickup(const FObjectInitializer& ObjectInitializer);

	/* Class to add to inventory when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
	TSubclassOf<ATBWeapon> WeaponClass;

	virtual void OnUsed(APawn* InstigatorPawn) override;
	
	
	
	
};
