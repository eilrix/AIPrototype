#include "Components/UnitsControlComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Units/UnitSpawner.h"
#include "AIPrototype/Public/AIPrototype.h"
#include "Kismet/GameplayStatics.h"

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
	checkf(m_OwningController.Get() != nullptr, TEXT("%hs: component should be owned only by AController that represents human or AI player."), __FUNCTION__);
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

	const auto owner_team_agent_interface = Cast<IGenericTeamAgentInterface>(owning_controller);
	checkf(owner_team_agent_interface != nullptr, TEXT("%hs: owning controller doesn't implement IGenericTeamAgentInterface, can't establish ownership of unit spawner and so ownership of units."), __FUNCTION__);
	
	FActorSpawnParameters spawn_parameters;
	spawn_parameters.Owner = GetOwner();
	spawn_parameters.ObjectFlags |= RF_Transient;
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawn_parameters.bDeferConstruction = true;

	m_UnitSpawner = GetWorld()->SpawnActor<AUnitSpawner>(unit_spawner_class, spawn_parameters);
	checkf(m_UnitSpawner != nullptr, TEXT("%hs: failed to spawn unit spawner."), __FUNCTION__);
	m_UnitSpawner->InitOwningPlayerTeamID(owner_team_agent_interface->GetGenericTeamId());

	const auto owner_start_spot = owning_controller->StartSpot; // always valid in this context, no need to check
	UGameplayStatics::FinishSpawningActor(m_UnitSpawner, FTransform(owner_start_spot->GetActorRotation(), owner_start_spot->GetActorLocation())); // spawn in owner start stop
	
	return true;
}



