// Fill out your copyright notice in the Description page of Project Settings.

#include "ContraPlayerController.h"
#include "ContraPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include <EnhancedInputSubsystems.h>

void AContraPlayerController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!ContraPlayer) return;

	if (ImpactVFX)
	{
		FVector PlayerSpawnLocation = DamageCauser ? DamageCauser->GetActorLocation() : ContraPlayer->GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactVFX, PlayerSpawnLocation, FRotator::ZeroRotator);
	}

	FVector DeathLocation = ContraPlayer->GetActorLocation();
	ContraPlayer->ApplyDamage(DamageAmount, DamageCauser);

	if (!ContraPlayer->IsAlive() && DeathVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathVFX, DeathLocation, FRotator::ZeroRotator);
	}
}

bool AContraPlayerController::IsAlive_Implementation() const
{
	if (ContraPlayer)
	{
		return ContraPlayer->IsAlive();
	}
	return false;
}

void AContraPlayerController::PlayerDeath()
{
	if (!GameMode)
	{
		return;
	}

	GameMode->SetLives(0);
    bAutoManageActiveCameraTarget = false;
	DetachFromPawn();
}

void AContraPlayerController::PlayerRespawn()
{
	GameMode->SetLives(GameMode->Lives - 1);
}

void AContraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

    GameMode = Cast<AContraGameMode>(GetWorld()->GetAuthGameMode());
	ContraPlayer = Cast<AContraPlayer>(GetPawn());

	if (GameMode)
	{
		GameMode->OnScoreChanged.AddDynamic(this, &AContraPlayerController::HandleScoreChanged);
		GameMode->OnLivesChanged.AddDynamic(this, &AContraPlayerController::HandleLivesChanged);

		// Push initial values to the HUD now that we're subscribed
		HandleScoreChanged(GameMode->Score);
		HandleLivesChanged(GameMode->Lives);
	}
}

void AContraPlayerController::HandleScoreChanged(int32 NewScore)
{
	if (HUDWidget)
	{
		HUDWidget->SetScore(NewScore);
	}
}

void AContraPlayerController::HandleLivesChanged(int32 NewLives)
{
	if (HUDWidget)
	{
		HUDWidget->SetLives(NewLives);
	}
}
