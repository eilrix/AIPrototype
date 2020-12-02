// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/AI/UnitAIController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void AUnitAIController::MoveToLocation(const FVector& Location)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Location);
}
