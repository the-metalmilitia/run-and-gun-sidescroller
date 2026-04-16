// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ContraGameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "ContraPlayer.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ContraPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ControlledPawn = GetPawn();

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

	
	FVector2D ScreenPos;
	if (ContraPlayerController && UGameplayStatics::ProjectWorldToScreen(ContraPlayerController, ControlledPawn->GetActorLocation(), ScreenPos, false))
	{
		if (ScreenPos.X < -1000.f)
		{
			DestroyPawn(); // culled off-screen, no score
		}
	}
}

void AEnemyAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive_Implementation() || DamageCauser == GetPawn()) return;

	Health = FMath::Max(0.f, Health - DamageAmount);

	if (ImpactVFX && GetPawn())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVFX, GetPawn()->GetActorLocation(), FRotator::ZeroRotator);
	}

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

	if (ControlledPawn)
	{
		FVector PawnLocation = ControlledPawn->GetActorLocation();

		if (DeathVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathVFX, PawnLocation, FRotator::ZeroRotator);
		}

		if (AContraPlayer* ContraPlayer = Cast<AContraPlayer>(ControlledPawn))
		{
			ContraPlayer->PlayDeathSound();
		}
	}

	DestroyPawn();
}

void AEnemyAIController::DestroyPawn()
{
	if (!ControlledPawn) return;

	TArray<AActor*> AttachedActors;
	ControlledPawn->GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		if (Actor) Actor->Destroy();
	}

	ControlledPawn->Destroy();
}

void AEnemyAIController::SetBehaviorTree(UBehaviorTree* NewBehaviorTree)
{
	BehaviorTree = NewBehaviorTree;
	if (BehaviorTree && GetWorld())
	{
		RunBehaviorTree(BehaviorTree);
	}
}
