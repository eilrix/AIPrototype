// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/UnitBase.h"
#include "Controllers/UnitAIController.h"

AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AUnitAIController::StaticClass();
}

void AUnitBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	m_OwnerTeamID = TeamID;
}

FGenericTeamId AUnitBase::GetGenericTeamId() const
{
	return m_OwnerTeamID;
}

