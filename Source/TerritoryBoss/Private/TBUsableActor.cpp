// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBUsableActor.h"


// Sets default values
ATBUsableActor::ATBUsableActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
}

void ATBUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}

void ATBUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}

void ATBUsableActor::OnUsed(APawn* InstigatorPawn)
{
	// TODO ..
}


