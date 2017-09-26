// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBDamageType.h"

UTBDamageType::UTBDamageType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* We apply this modifier based on the physics material setup to the head of the enemy PhysAsset */
	HeadDmgModifier = 2.0f;
	LimbDmgModifier = 0.5f;
}

float UTBDamageType::GetHeadDamageModifier()
{
	return HeadDmgModifier;
}

float UTBDamageType::GetLimbDamageModifier()
{
	return LimbDmgModifier;
}



