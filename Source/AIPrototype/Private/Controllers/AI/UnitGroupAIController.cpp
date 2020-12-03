// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/AI/UnitGroupAIController.h"
#include "DelegateHelpers.h"
#include "Controllers/AI/UnitAIController.h"
#include "Units/UnitBase.h"

void AUnitGroupAIController::InitializeControlledUnits(const TArray<AUnitBase*>& Units)
{
	for (const auto unit : Units)
	{
		m_ControlledUnits.AddUnique(unit);
	}
}

void AUnitGroupAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTreeAsset);
}

void AUnitGroupAIController::MoveGroupToLocation(const FVector& Location, const float AcceptanceRadius)
{
	for (const auto unit_weak_ptr : m_ControlledUnits)
	{
		if (const auto unit = unit_weak_ptr.Get())
		{
			const auto unit_ai_controller = Cast<AUnitAIController>(unit->GetController());
			if (unit_ai_controller != nullptr)
			{
				unit_ai_controller->MoveToLocation(Location);
			}
		}
	}

	auto pathFollowingComponent = GetPathFollowingComponent();
	pathFollowingComponent->SetAcceptanceRadius(AcceptanceRadius);
	Chain(this, pathFollowingComponent->OnRequestFinished, OnGroupMovementFinished, this);
}

