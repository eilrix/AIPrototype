// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/UnitBase.h"
#include "Controllers/AI/UnitAIController.h"

AUnitBase::AUnitBase()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AUnitAIController::StaticClass();
}

