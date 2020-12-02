// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/UnitSpawner.h"
#include "AIPrototype/AIPrototype.h"
#include "Controllers/AI/UnitGroupAIController.h"
#include "Editor/GroupActor.h"
#include "VisualLogger/VisualLogger.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Units/UnitBase.h"

AUnitSpawner::AUnitSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUnitSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnUnits();
}

bool AUnitSpawner::SpawnUnits()
{
	if (unit_class == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: unit class isn't specified, abort."), __FUNCTION__);
		return false;
	}

	if (unit_group_AI_controller_class == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: unit group AI controller class isn't specified, abort."), __FUNCTION__);
		return false;
	}

	// we spawn at least one unit, despite of the deviation
	const int32 num_units_to_spawn = FMath::RandRange(FMath::Max(1, units_number - units_number_random_deviation), units_number + units_number_random_deviation); 

	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = this;
	spawn_parameters.ObjectFlags |= RF_Transient;
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (int32 units_spawned = 0; units_spawned < num_units_to_spawn - 1; ++units_spawned) // last unit of the group will be commander
	{
		m_SpawnedUnits.Add(GetWorld()->SpawnActor<AUnitBase>(unit_class, GetActorLocation(), GetActorRotation(), spawn_parameters)); // TODO location random deviation for less collisions?
	}

	SpawnCommander(spawn_parameters);
	
	return true; // @TODO check if units are valid? don't crash
}

void AUnitSpawner::SpawnCommander(FActorSpawnParameters& SpawnParameters)
{
	SpawnParameters.bDeferConstruction = true;

	auto commander = GetWorld()->SpawnActor<AUnitBase>(unit_class, SpawnParameters);
	commander->AIControllerClass = unit_group_AI_controller_class;
	UGameplayStatics::FinishSpawningActor(commander, FTransform(GetActorLocation()));

	m_SpawnedUnits.Add(commander);
	Cast<AUnitGroupAIController>(commander->GetController())->InitializeControlledUnits(m_SpawnedUnits); // always valid
}
