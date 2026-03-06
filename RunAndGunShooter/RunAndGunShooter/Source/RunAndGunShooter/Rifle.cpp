// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

void ARifle::Shoot_Implementation()
{
	Super::Shoot_Implementation();
	ProjectileDataManager->GetProjectileFromPool(SpawnedProjectileType, ProjectilesPerShot, ProjectileAmountPerShot);

	for (AProjectileBase* projectile : ProjectilesPerShot)
	{
		if (projectile)
		{
			projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
			projectile->SetActorRotation(GetActorRotation());
			projectile->SetActorHiddenInGame(false);
			projectile->SetActorEnableCollision(true);
		}
	}
}
