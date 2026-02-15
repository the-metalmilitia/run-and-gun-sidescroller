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
		UE_LOG(LogTemp, Warning, TEXT("Type: %d, Amount: %d, QueueSizeCounter: %d Pool is Empty: %d, CurrentPoolType: %d")
			, Type, Amount, QueueSizeCounter, ProjectilePool.IsEmpty(), GetCurrentProjectileType());
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
			UE_LOG(LogTemp, Warning, TEXT("Projectile dequeued of type %d"), Type);
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
	QueueSizeCounter++;
}

void UProjectileDataManager::Initialize(UWorld* WorldContext)
{
	CurrentWorld = WorldContext;
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
		AProjectileBase* ProjectileToSpawn = GetWorld()->SpawnActor<AProjectileBase>(TypesOfProjectile[static_cast<int>(type)]->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator);;
		ProjectileToSpawn->SetActorEnableCollision(false);
		ProjectileToSpawn->SetActorHiddenInGame(true);
		ProjectileToSpawn->SetActorTickEnabled(false);
		UE_LOG(LogTemp, Warning, TEXT("ProjectileToSpawn: %s"), *ProjectileToSpawn->GetName());

		if(ProjectileToSpawn)
		{
			ProjectilePool.Enqueue(ProjectileToSpawn);
			QueueSizeCounter++;
			UE_LOG(LogTemp, Warning, TEXT("Projectile created of type %d"), type);
		}
		// if (AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(TypesOfProjectile[static_cast<int>(type)]->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, spawnParams))
		// {
		// 	ProjectilePool.Enqueue(Projectile);
		// 	QueueSizeCounter++;
		// }
	}
	CurrentPoolType = type;
}
