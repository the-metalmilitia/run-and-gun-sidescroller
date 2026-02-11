// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDataManager.h"

void UProjectileDataManager::GetProjectileFromPool(TSubclassOf<AProjectileBase> ProjectileClass,
	TArray<AProjectileBase*> Projectiles, int Amount)
{
	if (ProjectileClass == nullptr
		|| Amount <= 0
		|| ProjectilePool.IsEmpty()
		|| Amount > QueueSizeCounter
		|| Amount > MaxProjectilePoolSize)
	{
		return;
	}

	if (!ProjectilePool.IsEmpty())
	{
		Projectiles.Empty();
	}

	Projectiles.Reserve(Amount);

	while (!ProjectilePool.IsEmpty() && Amount > 0)
	{
		AProjectileBase* Projectile;
		if (ProjectilePool.Dequeue(Projectile) && Projectile->GetClass() == ProjectileClass)
		{
			Projectiles.Add(Projectile);
			Amount--;
			QueueSizeCounter--;
		}
	}
}

void UProjectileDataManager::ReturnProjectileToPool(AProjectileBase* Projectile)
{
	if (Projectile == nullptr
		|| Projectile->GetProjectileType() != GetCurrentProjectileType()
		|| QueueSizeCounter == MaxProjectilePoolSize)
	{
		return;
	}

	ProjectilePool.Enqueue(Projectile);
}

ProjectileType UProjectileDataManager::GetCurrentProjectileType() const
{
	if (ProjectilePool.IsEmpty())
	{
		return ProjectileType::Invalid;
	}
	
	return ((AProjectileBase*)ProjectilePool.Peek())->GetProjectileType();
}

void UProjectileDataManager::CreateProjectilePool(ProjectileType type)
{
	ProjectilePool.Empty();
	QueueSizeCounter = 0;
	for (int i = 0; i < MaxProjectilePoolSize; i++)
	{
		
		if (AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(TypesOfProjectile[(int)type]))
		{
			ProjectilePool.Enqueue(Projectile);
			QueueSizeCounter++;
		}
	}
}
