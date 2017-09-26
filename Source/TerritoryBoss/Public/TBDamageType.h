// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "TBDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API UTBDamageType : public UDamageType
{
	GENERATED_BODY()

	UTBDamageType(const FObjectInitializer& ObjectInitializer);

	/* Damage modifier for headshot damage */
	UPROPERTY(EditDefaultsOnly)
	float HeadDmgModifier;

	UPROPERTY(EditDefaultsOnly)
	float LimbDmgModifier;

public:

	float GetHeadDamageModifier();

	float GetLimbDamageModifier();
	
};
