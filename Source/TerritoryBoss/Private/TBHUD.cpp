// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "TerritoryBoss.h"
#include "../Public/TBHUD.h"
#include "TBCharacter.h"
#include "TBPlayerController.h"
#include "TBUsableActor.h"

ATBHUD::ATBHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDCenterDotObj(TEXT("/Game/UI/HUD/T_CenterDot_M.T_CenterDot_M"));
	CrosshairIcon = UCanvas::MakeIcon(HUDCenterDotObj.Object);
}

void ATBHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrosshair();
}

void ATBHUD::DrawCrosshair()
{
	float CenterX = Canvas->ClipX / 2;
	float CenterY = Canvas->ClipY / 2;

	float CrosshairScale = 0.07f;

	ATBPlayerController* PCOwner = Cast<ATBPlayerController>(PlayerOwner);
	if (PCOwner)
	{
		ATBCharacter* Pawn = Cast<ATBCharacter>(PCOwner->GetPawn());
		if (Pawn) // TODO: && Pawn->IsAlive() (Disabled this check until Death & Respawn is implemented)
		{
			// Boost size when hovering over a switchable object.
			ATBUsableActor* usable = Pawn->GetUsableInView();
			if (usable)
				CrosshairScale *= 1.5f;

			Canvas->SetDrawColor(255, 255, 255, 255);
			Canvas->DrawIcon(CrosshairIcon,
				CenterX - CrosshairIcon.UL*CrosshairScale / 2.0f,
				CenterY - CrosshairIcon.VL*CrosshairScale / 2.0f, CrosshairScale);
		}
	}
}