// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerEnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void ARunnerEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ContraPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(MovementCheckTimer, this, &ARunnerEnemyAIController::StartMovement, 0.2f, true);
}

void ARunnerEnemyAIController::StartMovement()
{
	ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	if (ControlledPawn->GetDistanceTo(PlayerPawn) > ThresholdAlertDistance) return;

	GetWorldTimerManager().ClearTimer(MovementCheckTimer);

	if (!ContraPlayerController) return;

	int32 ViewportSizeX, ViewportSizeY;
	ContraPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector WorldLocation, WorldDirection;
	ContraPlayerController->DeprojectScreenPositionToWorld(0.f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);

	FVector PawnLocation = ControlledPawn ? ControlledPawn->GetActorLocation() : FVector::ZeroVector;
	FVector TargetLocation(WorldLocation.X + 1000.0f, PawnLocation.Y, PawnLocation.Z);

	MoveToLocation(TargetLocation);
}