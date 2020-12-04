// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/AI/UnitGroupAIController.h"

#include "CommanderBlackboardDataKeys.h"
#include "DelegateHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/AI/UnitAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Units/UnitBase.h"

AUnitGroupAIController::AUnitGroupAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
}

void AUnitGroupAIController::InitializeControlledUnits(const TArray<AUnitBase*>& Units)
{
	for (const auto unit : Units)
	{
		m_ControlledUnits.AddUnique(unit);
	}

	InitBlackboardAlliesNum();
}

void AUnitGroupAIController::InitBlackboardAlliesNum()
{
	GetBlackboardComponent()->SetValueAsInt(CBBKeys::AlliesNum, m_ControlledUnits.Num());
}

void AUnitGroupAIController::BeginPlay()
{
	Super::BeginPlay();

	SubscribeOnPerceptionUpdates();
	RunBehaviorTree(BehaviorTreeAsset);
}

void AUnitGroupAIController::SubscribeOnPerceptionUpdates()
{
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AUnitGroupAIController::OnTargetPerceptionUpdated);
}

void AUnitGroupAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.IsActive())
	{
		if (!m_SensedEnemies.Contains(Actor))
		{
			m_SensedEnemies.AddUnique(Actor);
		}
	}
	else if (Stimulus.IsExpired())
	{
		if (m_SensedEnemies.Contains(Actor))
		{
			m_SensedEnemies.Remove(Actor);
		}
	}

	UpdateBlackboardEnemiesNum();
}

void AUnitGroupAIController::UpdateBlackboardEnemiesNum()
{
	GetBlackboardComponent()->SetValueAsInt(CBBKeys::EnemiesNum, m_SensedEnemies.Num());
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

