// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "TBCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYBOSS_API UTBCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	virtual float GetMaxSpeed() const override;
};
