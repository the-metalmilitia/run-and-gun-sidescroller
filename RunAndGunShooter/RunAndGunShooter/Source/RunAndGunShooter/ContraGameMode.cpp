// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraGameMode.h"

void AContraGameMode::StartPlay()
{
	Super::StartPlay();

	CurrentLives = MaxLives;
}