// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/AI/UnitAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIPrototype/AIPrototype.h"

void AUnitAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	const auto controlledPawn = GetPawn();
	if (controlledPawn->IsValidLowLevelFast())
	{
		// it's widely used practice to pass IGenericTeamAgentInterface calls from AI controller to controlled pawns, for simplicity of usage
		const auto pawn_team_agent_interface = Cast<IGenericTeamAgentInterface>(GetPawn());
		checkf(pawn_team_agent_interface != nullptr, TEXT("%hs: controlled pawn doesn't implement IGenericTeamAgentInterface. Every unit in the game must implement the interface to properly handle units ownership."), __FUNCTION__);
		pawn_team_agent_interface->SetGenericTeamId(NewTeamID);
	}
	else
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: doesn't control any valid pawn at the moment, operation failed."), __FUNCTION__);
	}
}

FGenericTeamId AUnitAIController::GetGenericTeamId() const
{
	const auto controlledPawn = GetPawn();
	if (controlledPawn->IsValidLowLevelFast())
	{
		const auto pawn_team_agent_interface = Cast<IGenericTeamAgentInterface>(GetPawn());
		checkf(pawn_team_agent_interface != nullptr, TEXT("%hs: controlled pawn doesn't implement IGenericTeamAgentInterface. Every unit in the game must implement the interface to properly handle units ownership."), __FUNCTION__);
		return pawn_team_agent_interface->GetGenericTeamId();
	}

	UE_LOG(LogAIPrototype, Error, TEXT("%hs: doesn't control any valid pawn at the moment, operation failed."), __FUNCTION__);
	return FGenericTeamId::NoTeam;
}

void AUnitAIController::MoveToLocation(const FVector& Location)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Location);
}
