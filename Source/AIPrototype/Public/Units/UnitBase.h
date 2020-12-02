// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitBase.generated.h"

UCLASS(Abstract)
class AIPROTOTYPE_API AUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	AUnitBase();
};
