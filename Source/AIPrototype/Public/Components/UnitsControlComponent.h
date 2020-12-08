#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitsControlComponent.generated.h"

class AUnitSpawner;
class AController;

/*
 * @brief Component that is responsible for everything related to units ownership and spawning.
 * @details Should be owned by unique human or AI player controller .
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROTOTYPE_API UUnitsControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitsControlComponent();

public:
	/* Unit spawner class that will be used to create spawner associated with component's owner on BeginPlay. */
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
