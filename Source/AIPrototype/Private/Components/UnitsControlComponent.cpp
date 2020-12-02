// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/UnitsControlComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Units/UnitSpawner.h"
#include "AIPrototype/AIPrototype.h"

UUnitsControlComponent::UUnitsControlComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUnitsControlComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheOwningController();
	CreateUnitSpawner();
}

void UUnitsControlComponent::CacheOwningController()
{
	m_OwningController = Cast<AController>(GetOwner());
	ensureMsgf(m_OwningController.Get() != nullptr, TEXT("%hs: component should be owned only by AController that represents a player - e.g APlayerController or AAIPlayerController."), __FUNCTION__);
}

bool UUnitsControlComponent::CreateUnitSpawner()
{
	if (unit_spawner_class == nullptr)
	{
		UE_LOG(LogAIPrototype, Error, TEXT("%hs: unit spawner class isn't specified, abort."), __FUNCTION__);
		return false;
	}
	
	const auto owning_controller = m_OwningController.Get();
	checkf(owning_controller != nullptr, TEXT("%hs: owning controller is nullptr, this should never happen."), __FUNCTION__);
	
	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = GetOwner();
	spawn_parameters.ObjectFlags |= RF_Transient;
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const auto owner_start_spot = owning_controller->StartSpot; // always valid in this context, there's no need to check
	m_UnitSpawner = GetWorld()->SpawnActor<AUnitSpawner>(unit_spawner_class, owner_start_spot->GetActorLocation(), owner_start_spot->GetActorRotation(), spawn_parameters);
	return m_UnitSpawner != nullptr;
}



