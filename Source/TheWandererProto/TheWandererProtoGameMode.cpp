// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheWandererProtoGameMode.h"
#include "TheWandererProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheWandererProtoGameMode::ATheWandererProtoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
