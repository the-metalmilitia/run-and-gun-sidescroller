// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDataManager.h"

void UProjectileDataManager::CreateProjectilePool(UObject* WorldContextObject, ProjectileType Type)
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;

	if (TypesOfProjectile.Num() == 0
		|| Type == ProjectileType::Invalid
		|| static_cast<int32>(Type) >= TypesOfProjectile.Num()
		|| World == nullptr)
	{
		return;
	}

	TArray<AProjectileBase*>& Pool = ProjectilePools.FindOrAdd(static_cast<int32>(Type));

	for (AProjectileBase* Projectile : Pool)
	{
		if (IsValid(Projectile)) Projectile->Destroy();
	}
	Pool.Empty();
	Pool.Reserve(PoolSize);

	for (int32 i = 0; i < PoolSize; i++)
	{
		AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(
			TypesOfProjectile[static_cast<int32>(Type)], FVector::ZeroVector, FRotator::ZeroRotator);

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
		for (AProjectileBase* Projectile : Pair.Value)
		{
			if (IsValid(Projectile)) Projectile->Destroy();
		}
	}
	ProjectilePools.Empty();
}

void UProjectileDataManager::GetProjectileFromPool(ProjectileType Type, TArray<AProjectileBase*>& Projectiles, int Amount)
{
	TArray<AProjectileBase*>* Pool = ProjectilePools.Find(static_cast<int32>(Type));

	if (!Pool
		|| Type == ProjectileType::Invalid
		|| Amount <= 0
		|| Pool->Num() == 0
		|| Amount > Pool->Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetProjectileFromPool failed: Type=%d, Amount=%d, PoolSize=%d"),
			Type, Amount, Pool ? Pool->Num() : 0);
		return;
	}

	Projectiles.Empty();
	Projectiles.Reserve(Amount);

	while (Amount > 0 && Pool->Num() > 0)
	{
		Projectiles.Add(Pool->Pop());
		Amount--;
	}
}

void UProjectileDataManager::ReturnProjectileToPool(AProjectileBase* Projectile)
{
	if (!IsValid(Projectile)) return;

	TArray<AProjectileBase*>* Pool = ProjectilePools.Find(static_cast<int32>(Projectile->GetProjectileType()));

	if (!Pool || Pool->Num() >= PoolSize) return;

	Projectile->Activate(false);
	Pool->Add(Projectile);
}
