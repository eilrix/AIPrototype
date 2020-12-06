// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GameFramework/AIPrototypeGameMode.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/AIPrototypeGameInstance.h"
#include "Controllers/AI/AIPlayerController.h"
#include "AIPrototype/AIPrototype.h"
#include "Engine/PlayerStartPIE.h"
#include "Kismet/GameplayStatics.h"
#include "AIPrototype/Public/GameFramework/UniquePlayerStart.h"

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
	IGenericTeamAgentInterface* player_team_agent_interface = nullptr;

	// I just give away team ID's locally here. Proper team ID logic, considering human and AI players, and teams, would take a while to implement. So using that approach for simplicity.
	uint8 player_team_ID = 1; 
	
	for (int32 num_ai_players_spawned = 0; num_ai_players_spawned < number_of_AI_players; ++num_ai_players_spawned)
	{
		AI_player = GetWorld()->SpawnActor<AAIPlayerController>(AI_player_controller_class, spawn_parameters);
		checkf(AI_player != nullptr, TEXT("%hs: failed to spawn AI Player of class %s, this should never happen."), __FUNCTION__, *AI_player_controller_class->GetName());
		
		AI_player_start_actor = FindPlayerStart(AI_player);
		checkf(AI_player_start_actor != nullptr, TEXT("%hs: failed to find any player start actor, this should never happen."), __FUNCTION__);

		AI_player->StartSpot = AI_player_start_actor;

		// init AI player team ID
		player_team_agent_interface = Cast<IGenericTeamAgentInterface>(AI_player);
		checkf(player_team_agent_interface != nullptr, 
			TEXT("%hs: create AI player doesn't implement IGenericTeamAgentInterfac, but it absolutely have to. You have provided improper AI_player_controller_class."), __FUNCTION__);
		player_team_agent_interface->SetGenericTeamId(player_team_ID);

		UGameplayStatics::FinishSpawningActor(AI_player, FTransform(AI_player_start_actor->GetActorRotation(), AI_player_start_actor->GetActorLocation()));

		// register AI player creation
		game_instance->AddAIPlayer(AI_player);
		++NumBots;
		++player_team_ID;
	}

	return true;
}

AActor* AAIPrototypeGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UWorld* world = GetWorld();
	AActor* selected_player_start = nullptr;
	const bool bSelectingForHumanPlayer = Cast<APlayerController>(Player) != nullptr;

	for (TActorIterator<APlayerStart> player_start_iterator(world); player_start_iterator; ++player_start_iterator)
	{
		const auto player_start = *player_start_iterator;

		if (bSelectingForHumanPlayer && player_start->IsA<APlayerStartPIE>())
		{
			// always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			selected_player_start = player_start;
			break;
		}

		auto unique_player_start = Cast<AUniquePlayerStart>(player_start);
		if (bSelectingForHumanPlayer)
		{
			// don't use unique player start for human player spawning. That is so only to easily configure spectating player position for this demo.
			if (unique_player_start == nullptr)
			{
				selected_player_start = player_start;
				break;
			}
		}
		// use unoccupied unique player starts to spawn AI player. That way AI players will never start in identical location
		else if (unique_player_start != nullptr && !unique_player_start->IsOccupied())
		{
			unique_player_start->SetIsOccupied(true);
			selected_player_start = unique_player_start;
			break;
		}
	}

	if (selected_player_start == nullptr)
	{
		UE_LOG(LogAIPrototype, Error,
			TEXT("%hs: failed to select valid player start. It's either no regular player start for human players or not enough unique player stars for desired number of AI players. Default player start selection logic will be used."),
			__FUNCTION__);
		selected_player_start = Super::ChoosePlayerStart_Implementation(Player);
	}

	return selected_player_start;
}

