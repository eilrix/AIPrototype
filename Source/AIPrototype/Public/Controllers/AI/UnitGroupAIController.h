// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAIController.h"
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
	UFUNCTION(BlueprintCallable)
	void MoveGroupToLocation(const FVector& Location, float AcceptanceRadius);
	void InitializeControlledUnits(const TArray<AUnitBase*>& Units);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(BlueprintAssignable)
	FOnGroupMovementFinished OnGroupMovementFinished;

protected:
	virtual void BeginPlay() override;

private:
	// Controlled units, including self
	TArray<TWeakObjectPtr<AUnitBase>> m_ControlledUnits;
};
