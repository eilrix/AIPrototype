#pragma once

#include "CoreMinimal.h"
#include "UnitAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "UnitGroupAIController.generated.h"

class AUnitBase;
class AUnitGroupAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroupMovementFinished, AUnitGroupAIController*, UnitGroupAIController);

/**
 * @brief Represents AI controller that commands over a group of units. Controlled pawn (commander) belongs to controlled group.
 */
UCLASS()
class AIPROTOTYPE_API AUnitGroupAIController : public AUnitAIController
{
	GENERATED_BODY()

public:
	AUnitGroupAIController();

	/* Initializes controlled units array with given units. */
	void InitializeControlledUnits(const TArray<AUnitBase*>& Units);
	
	/* Move controlled group to given location within acceptance radius. */
	UFUNCTION(BlueprintCallable)
	void MoveGroupToLocation(const FVector& Location, float AcceptanceRadius);

	virtual void Tick(float DeltaSeconds) override;
	
public:
	/* BT asset that will be executed on BeginPlay */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UBehaviorTree* BehaviorTreeAsset;

	/* For simplicity we consider group movement finished, when commander reaches current goal location. */
	UPROPERTY(BlueprintAssignable)
	FOnGroupMovementFinished OnGroupMovementFinishedDelegate;

protected:
	virtual void BeginPlay() override;
	
	void UpdateBlackboardEnemyLocation();
	FVector CalcSensedEnemiesCenterLocation() const;
	void UpdateBlackboardEnemiesNum();

protected:
	// Controlled units, including controlled pawn
	TArray<TWeakObjectPtr<AUnitBase>> m_ControlledUnits;
	TArray<TWeakObjectPtr<AActor>> m_SensedEnemies;
	
private:
	void InitBlackboardAlliesNum();
	void BindDelegates();
	
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	virtual void OnGroupMovementFinished(AUnitGroupAIController* UnitGroupAIController);
	
private:
	float m_OriginalAcceptanceRaidus = 0;
};
