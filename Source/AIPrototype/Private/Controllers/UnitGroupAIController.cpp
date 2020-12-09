#include "Controllers/UnitGroupAIController.h"
#include "Misc/CommanderBlackboardKeys.h"
#include "Misc/DelegateHelpers.h"
#include "AIPrototype/Public/AIPrototype.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/UnitAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Units/UnitBase.h"
#include "VisualLogger/VisualLogger.h"

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
	GetBlackboardComponent()->SetValueAsInt(CommanderBBKeys::AlliesNum, m_ControlledUnits.Num());
}

void AUnitGroupAIController::BeginPlay()
{
	Super::BeginPlay();

	BindDelegates();
	RunBehaviorTree(BehaviorTreeAsset);
}

void AUnitGroupAIController::BindDelegates()
{
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AUnitGroupAIController::OnTargetPerceptionUpdated);

	// consider group movement finished when commander reaches goal location
	Chain(this, GetPathFollowingComponent()->OnRequestFinished, OnGroupMovementFinishedDelegate, this);
	OnGroupMovementFinishedDelegate.AddDynamic(this, &AUnitGroupAIController::OnGroupMovementFinished);
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
	GetBlackboardComponent()->SetValueAsInt(CommanderBBKeys::EnemiesNum, m_SensedEnemies.Num());
}

void AUnitGroupAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateBlackboardEnemyLocation();
}

void AUnitGroupAIController::UpdateBlackboardEnemyLocation()
{
	if (m_SensedEnemies.Num() > 0)
	{
		GetBlackboardComponent()->SetValueAsVector(CommanderBBKeys::EnemyLocation, CalcSensedEnemiesCenterLocation());
	}
	else
	{
		GetBlackboardComponent()->ClearValue(CommanderBBKeys::EnemyLocation);
	}
}

FVector AUnitGroupAIController::CalcSensedEnemiesCenterLocation() const
{
	FVector min_location = FVector(BIG_NUMBER);
	FVector max_location = FVector(-BIG_NUMBER);
	FVector enemy_location = FVector::ZeroVector;

	for (const auto enemy_unit : m_SensedEnemies)
	{
		if (enemy_unit.IsValid())
		{
			enemy_location = enemy_unit->GetActorLocation();

			min_location.X = FMath::Min<float>(enemy_location.X, min_location.X);
			min_location.Y = FMath::Min<float>(enemy_location.Y, min_location.Y);
			min_location.Z = FMath::Min<float>(enemy_location.Z, min_location.Z);

			max_location.X = FMath::Max<float>(enemy_location.X, max_location.X);
			max_location.Y = FMath::Max<float>(enemy_location.Y, max_location.Y);
			max_location.Z = FMath::Max<float>(enemy_location.Z, max_location.Z);
		}
	}

	const auto center_location = (min_location + max_location) * 0.5f;

	UE_VLOG_BOX(this, LogAIPrototype, Log, FBox(min_location, max_location), FColor::Green, TEXT(""));
	UE_VLOG_SEGMENT(this, LogAIPrototype, Log, min_location, max_location, FColor::Blue, TEXT(""));
	UE_VLOG_LOCATION(this, LogAIPrototype, Log, center_location, 10, FColor::Red, TEXT("group center location"));

	return center_location;
}

void AUnitGroupAIController::MoveGroupToLocation(const FVector& Location, const float AcceptanceRadius)
{
	const auto pathFollowingComponent = GetPathFollowingComponent();
	m_OriginalAcceptanceRaidus = pathFollowingComponent->GetAcceptanceRadius();
	pathFollowingComponent->SetAcceptanceRadius(AcceptanceRadius);
	
	for (const auto unit : m_ControlledUnits)
	{
		if (unit.IsValid())
		{
			const auto unit_ai_controller = Cast<AUnitAIController>(unit->GetController());
			if (unit_ai_controller != nullptr)
			{
				unit_ai_controller->MoveToLocation(Location);
			}
			else
			{
				UE_LOG(LogAIPrototype, Warning, TEXT("%hs: unit '%s' isn't controlled by AUnitAIController, can't move it."), __FUNCTION__, *unit->GetName())
			}
		}
	}
}

void AUnitGroupAIController::OnGroupMovementFinished(AUnitGroupAIController* UnitGroupAIController)
{
	GetPathFollowingComponent()->SetAcceptanceRadius(m_OriginalAcceptanceRaidus);
}