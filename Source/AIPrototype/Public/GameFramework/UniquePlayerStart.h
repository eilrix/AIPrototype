// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "UniquePlayerStart.generated.h"

/**
 * @brief PlayerStart for one, unique player. If UniquePlayerStart is occupied by some player, other players can't use it as their start spot.
 */
UCLASS()
class AIPROTOTYPE_API AUniquePlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	bool IsOccupied() const;
	void SetIsOccupied(bool bReserved);
	
protected:
	bool m_bOccupied = false;
};
