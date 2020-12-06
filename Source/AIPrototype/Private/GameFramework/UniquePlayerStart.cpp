// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPrototype/Public/GameFramework/UniquePlayerStart.h"

bool AUniquePlayerStart::IsOccupied() const
{
	return m_bOccupied;
}

void AUniquePlayerStart::SetIsOccupied(bool bReserved)
{
	m_bOccupied = bReserved;
}
