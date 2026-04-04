// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ContraPlayer.h"

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

void ARunnerEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenPos;
	if (PC && UGameplayStatics::ProjectWorldToScreen(PC, ControlledPawn->GetActorLocation(), ScreenPos, false))
	{
		if (ScreenPos.X < 0.f)
		{
			Die();
		}
	}
}

void ARunnerEnemyAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive_Implementation() || DamageCauser == GetPawn()) return;

	if (Cast<AContraPlayer>(DamageCauser))
	{
		UE_LOG(LogTemp, Warning, TEXT("RunnerEnemy hit by player!"));
	}

	Health = FMath::Max(0.f, Health - DamageAmount);

	if (!IsAlive_Implementation())
	{
		Die();
	}
}

bool ARunnerEnemyAIController::IsAlive_Implementation() const
{
	return Health > 0.f;
}

void ARunnerEnemyAIController::Die()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->Destroy();
	}
}
