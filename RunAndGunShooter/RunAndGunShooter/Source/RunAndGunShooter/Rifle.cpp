// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

void ARifle::Shoot_Implementation()
{
	ProjectileDataManager->GetProjectileFromPool(SpawnedProjectileType, ProjectilesPerShot, ProjectileAmountPerShot);

	for (AProjectileBase* Projectile : ProjectilesPerShot)
	{
		if (Projectile)
		{
			Projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
			Projectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
			Projectile->SetOwningPool(ProjectileDataManager);
			Projectile->Activate(true);
		}
	}
}
