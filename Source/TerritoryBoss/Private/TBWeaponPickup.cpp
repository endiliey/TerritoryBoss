// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBWeapon.h"
#include "TBWeaponPickup.h"
#include "TBCharacter.h"
#include "TBPlayerController.h"

ATBWeaponPickup::ATBWeaponPickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void ATBWeaponPickup::OnUsed(APawn* InstigatorPawn)
{
	ATBCharacter* MyPawn = Cast<ATBCharacter>(InstigatorPawn);
	if (MyPawn)
	{
		/* Fetch the default variables of the class we are about to pick up and check if the storage slot is available on the pawn. */
		if (MyPawn->WeaponSlotAvailable(WeaponClass->GetDefaultObject<ATBWeapon>()->GetStorageSlot()))
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ATBWeapon* NewWeapon = GetWorld()->SpawnActor<ATBWeapon>(WeaponClass, SpawnInfo);

			MyPawn->AddWeapon(NewWeapon);

			Super::OnUsed(InstigatorPawn);
		}
		else
		{
			ATBPlayerController* PC = Cast<ATBPlayerController>(MyPawn->GetController());
			if (PC)
			{
				UE_LOG(LogTemp, Warning, TEXT("Weapon slot taken"));
			}
		}
	}
}




