// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AIPrototypeGameMode.h"
#include "AIPrototypePlayerController.h"
#include "AIPrototypeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAIPrototypeGameMode::AAIPrototypeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAIPrototypePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}