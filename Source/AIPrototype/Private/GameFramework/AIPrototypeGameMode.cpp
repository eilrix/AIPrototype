// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GameFramework/AIPrototypeGameMode.h"
#include "Engine/World.h"
#include "GameFramework/AIPrototypeGameInstance.h"
#include "Controllers/AI/AIPlayerController.h"
#include "AIPrototype/AIPrototype.h"
#include "Kismet/GameplayStatics.h"

AAIPrototypeGameMode::AAIPrototypeGameMode()
{
	AI_player_controller_class = AAIPlayerController::StaticClass();
}

void AAIPrototypeGameMode::StartPlay()
{
	Super::StartPlay();

	if (number_of_AI_players > 0)
	{
		SpawnAIPlayers();
	}
}

bool AAIPrototypeGameMode::SpawnAIPlayers()
{
	const auto game_instance = Cast<UAIPrototypeGameInstance>(GetGameInstance());
	if (game_instance == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: game instance isn't of %s type, failed to spawn AI players."), __FUNCTION__, *UAIPrototypeGameInstance::StaticClass()->GetName());
		return false;
	}

	if (AI_player_controller_class == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: AI player controller class isn't specified, abort."), __FUNCTION__);
		return false;
	}

	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = this;
	spawn_parameters.ObjectFlags |= RF_Transient; // We never want to save AI player controllers into a map
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawn_parameters.bDeferConstruction = true;

	AAIPlayerController* AI_player = nullptr;
	AActor* AI_player_start_actor = nullptr;

	for (int32 num_ai_players_spawned = 0; num_ai_players_spawned < number_of_AI_players; ++num_ai_players_spawned)
	{
		AI_player = GetWorld()->SpawnActor<AAIPlayerController>(AI_player_controller_class, FVector::ZeroVector, FRotator::ZeroRotator, spawn_parameters);
		checkf(AI_player != nullptr, TEXT("%hs: failed to spawn AI Player of class %s, this should never happen."), __FUNCTION__, *AI_player_controller_class->GetName());
		
		AI_player_start_actor = FindPlayerStart(AI_player);
		checkf(AI_player_start_actor != nullptr, TEXT("%hs: failed to find any player start actor, this should never happen."), __FUNCTION__);

		AI_player->StartSpot = AI_player_start_actor;
		UGameplayStatics::FinishSpawningActor(AI_player, FTransform(AI_player_start_actor->GetActorRotation(), AI_player_start_actor->GetActorLocation()));
		
		game_instance->AddAIPlayer(AI_player);
		++NumBots;
	}

	return true;
}


