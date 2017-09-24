// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBPickupActor.h"


ATBPickupActor::ATBPickupActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// All objects that can be picked up are simulated and can be thrown around the level
	MeshComp->SetSimulatePhysics(true);
	/* Ignore Pawn - this is to prevent objects shooting through the level or pawns glitching on top of small items. */
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;
}

void ATBPickupActor::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Remove Hack to workaround constructor call not currently working.
	MeshComp->SetSimulatePhysics(true);
}


void ATBPickupActor::OnUsed(APawn* InstigatorPawn)
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Super::OnUsed(InstigatorPawn);
}

