// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/UnitBase.h"
#include "Unit.generated.h"

// It should contain some logic that doesn't belong to base abstract class, but it doesn't (none logic left!) :). I kept it as instantiable unit class, because it's architecturally correct
// and it's quite probable that some logic may appear here (100% in actual game project)
UCLASS(BlueprintType, Blueprintable)
class AIPROTOTYPE_API AUnit : public AUnitBase
{
	GENERATED_BODY()
	
};
