// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerEnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void ARunnerEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorldTimerManager().SetTimerForNextTick(this, &ARunnerEnemyAIController::StartMovement);
}

void ARunnerEnemyAIController::StartMovement()
{
	MoveToLocation(PatrolDestination);
}

void ARunnerEnemyAIController::AttackPlayer()
{
	Super::AttackPlayer();
}
