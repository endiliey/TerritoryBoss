// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TBUsableActor.h"
#include "TBPickupActor.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API ATBPickupActor : public ATBUsableActor
{
	GENERATED_BODY()

public:

	ATBPickupActor(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	virtual void OnUsed(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* PickupSound;

};
