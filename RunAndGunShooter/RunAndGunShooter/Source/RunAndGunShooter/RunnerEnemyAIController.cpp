// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerEnemyAIController.h"

void ARunnerEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorldTimerManager().SetTimerForNextTick(this, &ARunnerEnemyAIController::StartMovement);
}

void ARunnerEnemyAIController::StartMovement()
{
	MoveToLocation(FVector(-2100, -2360, 343));
}

void ARunnerEnemyAIController::AttackPlayer()
{
	Super::AttackPlayer();
}
