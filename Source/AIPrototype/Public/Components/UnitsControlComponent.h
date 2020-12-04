// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitsControlComponent.generated.h"

class AUnitSpawner;
class AController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROTOTYPE_API UUnitsControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitsControlComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<AUnitSpawner> unit_spawner_class;

protected:
	virtual void BeginPlay() override;
	virtual bool CreateUnitSpawner();

protected:
	UPROPERTY(Transient)
	AUnitSpawner* m_UnitSpawner;
	
	TWeakObjectPtr<AController> m_OwningController;

private:
	void CacheOwningController();
};
