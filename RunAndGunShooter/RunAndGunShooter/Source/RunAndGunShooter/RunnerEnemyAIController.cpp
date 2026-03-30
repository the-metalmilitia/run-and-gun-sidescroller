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
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector WorldLocation, WorldDirection;
	PC->DeprojectScreenPositionToWorld(0.f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);

	APawn* ControlledPawn = GetPawn();
	FVector PawnLocation = ControlledPawn ? ControlledPawn->GetActorLocation() : FVector::ZeroVector;
	FVector TargetLocation(WorldLocation.X, PawnLocation.Y, PawnLocation.Z);

	MoveToLocation(TargetLocation);
}

void ARunnerEnemyAIController::AttackPlayer()
{
	Super::AttackPlayer();
}
