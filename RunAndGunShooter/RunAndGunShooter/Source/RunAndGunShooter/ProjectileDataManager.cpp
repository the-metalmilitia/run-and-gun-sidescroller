// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDataManager.h"

void UProjectileDataManager::GetProjectileFromPool(ProjectileType Type,
	TArray<AProjectileBase*> Projectiles, int Amount)
{
	if (Type == ProjectileType::Invalid
		|| Type != GetCurrentProjectileType()
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
		if (ProjectilePool.Dequeue(Projectile))
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

	if(TypesOfProjectile.Num() == 0
		|| type == ProjectileType::Invalid
		|| type >= TypesOfProjectile.Num()
		|| GetWorld() == nullptr)
	{
		return;
	}
	
	ProjectilePool.Empty();
	QueueSizeCounter = 0;
	for (int i = 0; i < MaxProjectilePoolSize; i++)
	{
		AProjectileBase* ProjectileToSpawn = GetWorld()->SpawnActor<AProjectileBase>();
		UE_LOG(LogTemp, Warning, TEXT("ProjectileToSpawn: %s"), *ProjectileToSpawn->GetName());

		if(ProjectileToSpawn)
		{
			ProjectilePool.Enqueue(ProjectileToSpawn);
			// 	QueueSizeCounter++;
		}
		// if (AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(TypesOfProjectile[static_cast<int>(type)]->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, spawnParams))
		// {
		// 	ProjectilePool.Enqueue(Projectile);
		// 	QueueSizeCounter++;
		// }
	}
}
