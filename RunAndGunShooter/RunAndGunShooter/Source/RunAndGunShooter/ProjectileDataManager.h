// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileBase.h"
#include "ProjectileDataManager.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class RUNANDGUNSHOOTER_API UProjectileDataManager : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
	void CreateProjectilePool(UObject* WorldContextObject, ProjectileType Type);

	UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
	void GetProjectileFromPool(ProjectileType Type, TArray<AProjectileBase*>& Projectiles, int Amount);

	UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
	void ReturnProjectileToPool(AProjectileBase* Projectile);

	UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
	void ClearPools();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Pool")
	TArray<TSubclassOf<AProjectileBase>> TypesOfProjectile;

	static constexpr int32 PoolSize = 20;

	TMap<int32, TArray<AProjectileBase*>> ProjectilePools;
};
