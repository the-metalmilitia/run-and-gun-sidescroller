// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ContraPlayer.h"
#include "Components/SkeletalMeshComponent.h"

ASniperAIController::ASniperAIController()
{
	ProjectileDataManager = CreateDefaultSubobject<UProjectileDataManager>("ProjectileDataManager");
}

void ASniperAIController::SetProjectile_Implementation(ProjectileType Type, int AmountPerShot)
{
	SpawnedProjectileType = Type;
	if (IsValid(ProjectileDataManager))
	{
		ProjectileDataManager->CreateProjectilePool(this, Type);
	}
}

void ASniperAIController::Shoot_Implementation()
{
}

UProjectileDataManager* ASniperAIController::GetProjectileDataManager_Implementation() const
{
	return ProjectileDataManager;
}

void ASniperAIController::Tick(float DeltaTime)
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
			ControlledPawn->Destroy();
		}
	}
}

void ASniperAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive_Implementation() || DamageCauser == GetPawn()) return;

	if (Cast<AContraPlayer>(DamageCauser))
	{
		UE_LOG(LogTemp, Warning, TEXT("SniperEnemy hit by player!"));
	}

	Health = FMath::Max(0.f, Health - DamageAmount);

	if (!IsAlive_Implementation())
	{
		Die();
	}
}

bool ASniperAIController::IsAlive_Implementation() const
{
	return Health > 0.f;
}

void ASniperAIController::Die()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->Destroy();
	}
}
