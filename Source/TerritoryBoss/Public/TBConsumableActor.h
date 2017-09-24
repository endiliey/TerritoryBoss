// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBPickupActor.h"
#include "TBConsumableActor.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API ATBConsumableActor : public ATBPickupActor
{
	GENERATED_UCLASS_BODY()

protected:

	/* Consume item, restoring energy to player */
	virtual void OnUsed(APawn* InstigatorPawn) override;

	/* Amount of hitpoints restored and hunger reduced when consumed. */
	UPROPERTY(EditDefaultsOnly, Category = "Consumable")
	float Nutrition;

};
