// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperAIController.h"
#include "GameFramework/Character.h"

void ASniperAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (ACharacter* Pawn = Cast<ACharacter>(GetPawn()))
	{
		if (Pawn->bIsCrouched) return;
	}
	Super::ApplyDamage_Implementation(DamageAmount, DamageCauser);
}

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
