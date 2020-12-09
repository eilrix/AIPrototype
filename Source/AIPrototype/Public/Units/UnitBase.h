// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "UnitBase.generated.h"

/*
 * @brief Base abstract class for any unit in the game. 
 */
UCLASS(Abstract)
class AIPROTOTYPE_API AUnitBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AUnitBase();

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	FGenericTeamId m_OwnerTeamID;
};
