// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AIPrototypeGameInstance.generated.h"

class AAIPlayerController;

UCLASS()
class AIPROTOTYPE_API UAIPrototypeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void AddAIPlayer(AAIPlayerController* AIPlayer);
	
protected:
	UPROPERTY(Transient)
	TArray<AAIPlayerController*> m_AIPlayers;
};
