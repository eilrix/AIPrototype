// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "UnitGroupAIController.generated.h"

class AUnitBase;
class AUnitGroupAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroupMovementFinished, AUnitGroupAIController*, UnitGroupAIController);

/**
 * 
 */
UCLASS()
class AIPROTOTYPE_API AUnitGroupAIController : public AUnitAIController
{
	GENERATED_BODY()

public:
	AUnitGroupAIController();
	
	UFUNCTION(BlueprintCallable)
	void MoveGroupToLocation(const FVector& Location, float AcceptanceRadius);
	void InitializeControlledUnits(const TArray<AUnitBase*>& Units);

	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(BlueprintAssignable)
	FOnGroupMovementFinished OnGroupMovementFinished;

protected:
	virtual void BeginPlay() override;
	void SubscribeOnPerceptionUpdates();
	void UpdateBBEnemyGroupCenterLocation();

	void InitBlackboardAlliesNum();
	void UpdateBlackboardEnemiesNum();
	
private:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	FVector CalcEnemyGroupCenterLocation() const;
	
private:
	// Controlled units, including self
	TArray<TWeakObjectPtr<AUnitBase>> m_ControlledUnits; 
	TArray<AActor*> m_SensedEnemies;
};
