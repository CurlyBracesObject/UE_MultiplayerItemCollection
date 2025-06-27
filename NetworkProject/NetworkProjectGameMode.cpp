// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetworkProjectGameMode.h"
#include "NetworkProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANetworkProjectGameMode::ANetworkProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
