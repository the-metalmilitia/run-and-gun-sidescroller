// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Engine/DataAsset.h"
#include "ProjectileDataManager.generated.h"

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API UProjectileDataManager : public UDataAsset
{
	GENERATED_BODY()

public:
	void GetProjectileFromPool(TSubclassOf<AProjectileBase> ProjectileClass, TArray<AProjectileBase*> Projectiles, int Amount);
	void ReturnProjectileToPool(AProjectileBase* Projectile);
	void CreateProjectilePool(ProjectileType type);
	ProjectileType GetCurrentProjectileType() const;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AProjectileBase>> TypesOfProjectile;

	UPROPERTY(EditDefaultsOnly)
	int MaxProjectilePoolSize = 30;

	int QueueSizeCounter = 0;
	TQueue<AProjectileBase*> ProjectilePool;

private:
};
