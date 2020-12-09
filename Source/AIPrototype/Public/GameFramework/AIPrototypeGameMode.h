// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AIPrototypeGameMode.generated.h"

class AAIPrototypeGameInstance;
class AAIPlayerController;

/*
 * @brief Currently only contains logic regarding AI & human players spawning
 */
UCLASS(minimalapi)
class AAIPrototypeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AAIPrototypeGameMode();
	virtual void StartPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
public:
	/* Class of AI player controller that will be used to spawn AI players on StartPlay */
	UPROPERTY(EditDefaultsOnly, NoClear, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<AAIPlayerController> AI_player_controller_class;

	/* Number of AI players to be spawned */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 number_of_AI_players = 0;

protected:
	virtual bool SpawnAIPlayers();
};



