// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBConsumableActor.h"
#include "TBCharacter.h"


ATBConsumableActor::ATBConsumableActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// A default to tweak per food variation in Blueprint
	Nutrition = 25;
}


void ATBConsumableActor::OnUsed(APawn* InstigatorPawn)
{
	// Plays pickup sound from base class
	Super::OnUsed(InstigatorPawn);

	UE_LOG(LogTemp, Warning, TEXT("Used consumables"));

	ATBCharacter* Pawn = Cast<ATBCharacter>(InstigatorPawn);
	if (Pawn)
	{
		Pawn->ConsumeFood(Nutrition);
	}
	// Remove from level
	Destroy();
}

