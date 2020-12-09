#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

/**
 * @brief Simple AI controller for single unit
 */
UCLASS()
class AIPROTOTYPE_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	/* Moves controlled pawn to given location */
	void MoveToLocation(const FVector& Location);

//~ Begin IGenericTeamAgentInterface 
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
//~ End IGenericTeamAgentInterface 
};
