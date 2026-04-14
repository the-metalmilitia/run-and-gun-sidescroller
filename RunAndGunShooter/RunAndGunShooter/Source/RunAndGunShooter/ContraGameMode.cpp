// Fill out your copyright notice in the Description page of Project Settings.

#include "ContraGameMode.h"

void AContraGameMode::AddScore(int32 Amount)
{
	Score += Amount;
	OnScoreChanged.Broadcast(Score);
}

void AContraGameMode::SetLives(int32 NewLives)
{
	Lives = NewLives;
	OnLivesChanged.Broadcast(Lives);
}
