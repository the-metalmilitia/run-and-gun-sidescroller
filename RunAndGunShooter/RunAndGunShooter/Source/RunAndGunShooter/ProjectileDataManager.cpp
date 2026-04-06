// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileDataManager.h"

void UProjectileDataManager::CreateProjectilePool(UObject* WorldContextObject, ProjectileType Type)
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;

	if (!Config
		|| Config->TypesOfProjectile.Num() == 0
		|| Type == ProjectileType::Invalid
		|| static_cast<int32>(Type) >= Config->TypesOfProjectile.Num()
		|| World == nullptr)
	{
		return;
	}

	TArray<AProjectileBase*>& Pool = ProjectilePools.FindOrAdd(static_cast<int32>(Type)).Projectiles;

	if (Pool.Num() > 0)
	{
		for (AProjectileBase* Projectile : Pool)
		{
			if (IsValid(Projectile)) Projectile->Destroy();
		}
		Pool.Empty();
		Pool.Reserve(UProjectilePoolConfig::PoolSize);
	}

	for (int32 i = 0; i < UProjectilePoolConfig::PoolSize; i++)
	{
		AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(
			Config->TypesOfProjectile[static_cast<int32>(Type)], FVector::ZeroVector, FRotator::ZeroRotator);

		if (Projectile)
		{
			Projectile->Activate(false);
			Pool.Add(Projectile);
			UE_LOG(LogTemp, Warning, TEXT("Projectile created of type %d"), Type);
		}
	}
}

void UProjectileDataManager::ClearPools()
{
	for (auto& Pair : ProjectilePools)
	{
		for (AProjectileBase* Projectile : Pair.Value.Projectiles)
		{
			if (IsValid(Projectile)) Projectile->Destroy();
		}
	}
	ProjectilePools.Empty();
}

void UProjectileDataManager::GetProjectileFromPool(ProjectileType Type, TArray<AProjectileBase*>& Projectiles, int Amount)
{
	FProjectilePool* PoolEntry = ProjectilePools.Find(static_cast<int32>(Type));

	if (!PoolEntry
		|| Type == ProjectileType::Invalid
		|| Amount <= 0
		|| PoolEntry->Projectiles.Num() == 0
		|| Amount > PoolEntry->Projectiles.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetProjectileFromPool failed: Type=%d, Amount=%d, PoolSize=%d"),
			Type, Amount, PoolEntry ? PoolEntry->Projectiles.Num() : 0);
		return;
	}

	Projectiles.Empty();
	Projectiles.Reserve(Amount);

	while (Amount > 0 && PoolEntry->Projectiles.Num() > 0)
	{
		Projectiles.Add(PoolEntry->Projectiles.Pop());
		Amount--;
	}
}

void UProjectileDataManager::ReturnProjectileToPool(AProjectileBase* Projectile)
{
	if (!IsValid(Projectile))
		return;

	Projectile->Activate(false);

	FProjectilePool* PoolEntry = ProjectilePools.Find(static_cast<int32>(Projectile->GetProjectileType()));

	if (!PoolEntry)
		return;

	if (!Config || PoolEntry->Projectiles.Num() >= UProjectilePoolConfig::PoolSize)
		return;

	PoolEntry->Projectiles.Add(Projectile);
}
