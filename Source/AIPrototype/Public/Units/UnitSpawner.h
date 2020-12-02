// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UnitSpawner.generated.h"

class AUnitBase;
class AUnitGroupAIController;

UCLASS()
class AIPROTOTYPE_API AUnitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnitSpawner();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AUnitBase> unit_class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AUnitGroupAIController> unit_group_AI_controller_class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (UIMin = 0, ClampMin = 0))
	int32 units_number = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (UIMin = 0, ClampMin = 0))
	int32 units_number_random_deviation = 2;

protected:
	virtual void BeginPlay() override;
	void SpawnCommander(FActorSpawnParameters& SpawnParameters);
	bool SpawnUnits();

protected:
	UPROPERTY(Transient)
	TArray<AUnitBase*> m_SpawnedUnits;

	UPROPERTY(Transient)
	AUnitGroupAIController* m_UnitGroupAIController;

};
