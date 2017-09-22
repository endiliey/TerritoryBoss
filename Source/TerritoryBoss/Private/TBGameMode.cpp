// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TerritoryBoss.h"
#include "TBGameMode.h"
#include "TBCharacter.h"
#include "TBPlayerController.h"

ATBGameMode::ATBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> TBPlayerPawnBPClass(TEXT("/Game/Character/ThirdPersonCharacter"));
	if (TBPlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = TBPlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ATBPlayerController::StaticClass();
}
