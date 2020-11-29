// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPrototype/Public/GameFramework/AIPrototypeGameInstance.h"

void UAIPrototypeGameInstance::AddAIPlayer(AAIPlayerController* AIPlayer)
{
	m_AIPlayers.AddUnique(AIPlayer);
}
