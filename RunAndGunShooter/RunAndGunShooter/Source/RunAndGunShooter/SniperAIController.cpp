// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperAIController.h"

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
