// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter_GameGameMode.h"
#include "Shooter_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShooter_GameGameMode::AShooter_GameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
