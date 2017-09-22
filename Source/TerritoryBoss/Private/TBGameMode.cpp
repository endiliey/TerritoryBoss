// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TerritoryBoss.h"
#include "TBGameMode.h"
#include "TBCharacter.h"

ATBGameMode::ATBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
