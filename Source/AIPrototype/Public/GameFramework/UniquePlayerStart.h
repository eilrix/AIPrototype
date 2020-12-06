// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "UniquePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class AIPROTOTYPE_API AUniquePlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	bool IsOccupied() const;
	void SetIsOccupied(bool bReserved);
	
private:
	bool m_bOccupied = false;
};
