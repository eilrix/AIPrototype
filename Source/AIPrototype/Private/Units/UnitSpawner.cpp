// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/UnitSpawner.h"
#include "AIPrototype/AIPrototype.h"
#include "Controllers/AI/UnitGroupAIController.h"
#include "Editor/GroupActor.h"
#include "VisualLogger/VisualLogger.h"
#include "Engine/World.h"
#include "Units/UnitBase.h"

AUnitSpawner::AUnitSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUnitSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnUnitGroup();
}

void AUnitSpawner::SpawnUnitGroup()
{
	if (SpawnUnits())
	{
		SpawnUnitGroupAIController();
	}
}

bool AUnitSpawner::SpawnUnits()
{
	if (unit_class == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: unit class isn't specified, abort."), __FUNCTION__);
		return false;
	}
	
	const int32 num_units_to_spawn = FMath::RandRange(FMath::Max(0, units_number - units_number_random_deviation), units_number + units_number_random_deviation);

	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = this;
	spawn_parameters.ObjectFlags |= RF_Transient;
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	for (int32 units_spawned = 0; units_spawned < num_units_to_spawn; ++units_spawned)
	{
		m_SpawnedUnits.AddUnique(GetWorld()->SpawnActor<AUnitBase>(unit_class, GetActorLocation(), GetActorRotation(), spawn_parameters));
	}
	
	return true; // @TODO check if units are valid? don't crash
}

bool AUnitSpawner::SpawnUnitGroupAIController()
{
	if (unit_group_AI_controller_class == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: unit group AI controller class isn't specified, abort."), __FUNCTION__);
		return false;
	}
	
	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = this;
	spawn_parameters.ObjectFlags |= RF_Transient;
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// spawn and attach group AI Controller to a center location of a group, using arbitrary unit of the group for attachment
	const FVector group_controller_location = GetUnitGroupCenterLocation();
	m_UnitGroupAIController = GetWorld()->SpawnActor<AUnitGroupAIController>(unit_group_AI_controller_class, group_controller_location, FRotator::ZeroRotator, spawn_parameters);
	m_UnitGroupAIController->AttachToActor(m_SpawnedUnits[0], FAttachmentTransformRules::KeepWorldTransform); // always have at least one unit in that context
	
	return m_UnitGroupAIController != nullptr;
}

FVector AUnitSpawner::GetUnitGroupCenterLocation() const
{
	FVector min_location = FVector(BIG_NUMBER);
	FVector max_location = FVector(-BIG_NUMBER);
	FVector unit_location = FVector::ZeroVector;

	for (const auto unit : m_SpawnedUnits)
	{
		if (unit->IsValidLowLevelFast())
		{
			unit_location = unit->GetActorLocation();
			
			min_location.X = FMath::Min<float>(unit_location.X, min_location.X);
			min_location.Y = FMath::Min<float>(unit_location.Y, min_location.Y);
			min_location.Z = FMath::Min<float>(unit_location.Z, min_location.Z);
			
			max_location.X = FMath::Max<float>(unit_location.X, max_location.X);
			max_location.Y = FMath::Max<float>(unit_location.Y, max_location.Y);
			max_location.Z = FMath::Max<float>(unit_location.Z, max_location.Z);
		}
	}

	const auto center_location = (min_location + max_location) * 0.5f;
	
	UE_VLOG_BOX(this, LogAIPrototype, Log, FBox(min_location, max_location), FColor::Green, TEXT("unit group 2D bounding box"));
	UE_VLOG_SEGMENT(this, LogAIPrototype, Log, min_location, max_location, FColor::Blue, TEXT("box diagonal"));
	UE_VLOG_LOCATION(this, LogAIPrototype, Log, center_location, 25, FColor::Red, TEXT("center location"));

	return center_location;
}

void AUnitSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LogGroupAIControllerLocation();
}

void AUnitSpawner::LogGroupAIControllerLocation() const
{
	if (m_UnitGroupAIController->IsValidLowLevelFast())
	{
		UE_VLOG_LOCATION(this, LogAIPrototype, Log, m_UnitGroupAIController->K2_GetActorLocation(), 25, FColor::Red, TEXT("UnitGroupAIController location = %s"), *m_UnitGroupAIController->K2_GetActorLocation().ToString());
	}
}

