// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UnitSpawner.generated.h"

class AUnitBase;
class AUnitGroupAIController;

/*
 * @brief Actor that spawns group of units.
 * @details One unit of a group is appointed to be commander and so controls whole group behavior.
 */
UCLASS()
class AIPROTOTYPE_API AUnitSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnitSpawner();
	void InitOwningPlayerTeamID(const FGenericTeamId& TeamID);
	
public:
	/* Class of units that will be spawned on spawner's BeginPlay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AUnitBase> unit_class;

	/* Class of AI controller that will be used by group commander to control its behavior */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<AUnitGroupAIController> unit_group_AI_controller_class;

	/* Number of units to be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (UIMin = 0, ClampMin = 0))
	int32 units_number = 5;

	/* Additional random deviation to number of spawned units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (UIMin = 0, ClampMin = 0))
	int32 units_number_random_deviation = 2;

protected:
	virtual void BeginPlay() override;
	bool SpawnUnits();
	void SetUnitTeamID(AUnitBase* Unit);
	void SpawnCommander(FActorSpawnParameters& SpawnParameters);

protected:
	UPROPERTY(Transient)
	TArray<AUnitBase*> m_SpawnedUnits;

	FGenericTeamId m_OwningPlayerTeamID;

};
