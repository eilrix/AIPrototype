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

void AAIPrototypeGameMode::SpawnAIPlayers()
{
	const auto game_instance = Cast<UAIPrototypeGameInstance>(GetGameInstance());
	if (game_instance == nullptr)
	{
		UE_LOG(LogAIPrototype, Warning, TEXT("%hs: game instance isn't of %s type, failed to spawn AI players."), __FUNCTION__, *UAIPrototypeGameInstance::StaticClass()->GetName());
		return;
	}

	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = this;
	spawn_parameters.ObjectFlags |= RF_Transient; // We never want to save AI player controllers into a map
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawn_parameters.bDeferConstruction = true;

	AAIPlayerController* AI_player = nullptr;
	const AActor* player_start_actor = nullptr;

	for (int32 ai_players_spawned = 0; ai_players_spawned < number_of_AI_players; ++ai_players_spawned)
	{
		AI_player = GetWorld()->SpawnActor<AAIPlayerController>(AI_player_controller_class, FVector::ZeroVector, FRotator::ZeroRotator, spawn_parameters);
		checkf(AI_player != nullptr, TEXT("%hs: failed to spawn AI Player of class %s, this should never happen."), __FUNCTION__, AI_player_controller_class ? *AI_player_controller_class->GetName() : TEXT("NULL"));
		
		player_start_actor = FindPlayerStart(AI_player);
		checkf(player_start_actor != nullptr, TEXT("%hs: failed to find any player start actor, this should never happen."), __FUNCTION__);

		//AI_player->SetInitialLocationAndRotation(player_start_actor->GetActorLocation(), player_start_actor->GetActorRotation());
		UGameplayStatics::FinishSpawningActor(AI_player, FTransform(player_start_actor->GetActorRotation(), player_start_actor->GetActorLocation()));
		
		game_instance->AddAIPlayer(AI_player);
		++NumBots;
	}
}


