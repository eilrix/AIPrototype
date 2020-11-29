// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AIPrototypeGameMode.generated.h"

class AAIPrototypeGameInstance;
class AAIPlayerController;

UCLASS(minimalapi)
class AAIPrototypeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AAIPrototypeGameMode();
	virtual void StartPlay() override;
	
public:
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<AAIPlayerController> AI_player_controller_class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameMode")
	int32 number_of_AI_players = 0;

private:
	void SpawnAIPlayers();
};



