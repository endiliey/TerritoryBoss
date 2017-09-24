// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryBoss.h"
#include "TBUsableActor.h"


// Sets default values
ATBUsableActor::ATBUsableActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;
}

void ATBUsableActor::OnUsed(APawn* InstigatorPawn)
{
	// TODO ..
}

void ATBUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
	MeshComp->CustomDepthStencilValue = 255;

	UE_LOG(LogTemp, Warning, TEXT("render"));
}

void ATBUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);


	UE_LOG(LogTemp, Warning, TEXT("unrender"));

}


