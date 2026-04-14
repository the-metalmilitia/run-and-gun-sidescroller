// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ContraGameMode.h"
#include "NiagaraFunctionLibrary.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn not found!"));
		return;
	}

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree not assigned!"));
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	if (PlayerPawn)
	{
		if (PlayerPawn->GetDistanceTo(ControlledPawn) <= ThresholdAlertDistance && LineOfSightTo(PlayerPawn))
		{
			SetFocus(PlayerPawn);
		}
		else
		{
			ClearFocus(EAIFocusPriority::Default);
		}
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenPos;
	if (PC && UGameplayStatics::ProjectWorldToScreen(PC, ControlledPawn->GetActorLocation(), ScreenPos, false))
	{
		if (ScreenPos.X < 0.f)
		{
			ControlledPawn->Destroy(); // culled off-screen, no score
		}
	}
}

void AEnemyAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive_Implementation() || DamageCauser == GetPawn()) return;

	Health = FMath::Max(0.f, Health - DamageAmount);

	if (!IsAlive_Implementation())
	{
		Die();
	}
}

bool AEnemyAIController::IsAlive_Implementation() const
{
	return Health > 0.f;
}

void AEnemyAIController::Die()
{
	if (AContraGameMode* GameMode = Cast<AContraGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->AddScore(ScoreValue);
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->Destroy();
	}
}

void AEnemyAIController::SetBehaviorTree(UBehaviorTree* NewBehaviorTree)
{
	BehaviorTree = NewBehaviorTree;
	if (BehaviorTree && GetWorld())
	{
		RunBehaviorTree(BehaviorTree);
	}
}
