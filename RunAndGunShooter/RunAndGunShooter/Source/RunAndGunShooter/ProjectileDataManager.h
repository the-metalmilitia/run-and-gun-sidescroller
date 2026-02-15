// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Engine/DataAsset.h"
#include "ProjectileDataManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RUNANDGUNSHOOTER_API UProjectileDataManager : public UDataAsset
{
	GENERATED_BODY()

public:
	void Initialize(UWorld* WorldContext);
	void CreateProjectilePool(ProjectileType type);
	void GetProjectileFromPool(ProjectileType Type, TArray<AProjectileBase*> Projectiles, int Amount);

	UFUNCTION(BlueprintCallable)
	void ReturnProjectileToPool(AProjectileBase* Projectile);

	ProjectileType GetCurrentProjectileType() const { return CurrentPoolType; }
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AProjectileBase>> TypesOfProjectile;

	UPROPERTY(EditDefaultsOnly)
	int MaxProjectilePoolSize = 30;

	int QueueSizeCounter = 0;
	TQueue<AProjectileBase*> ProjectilePool;

	UWorld* GetWorld() const override
	{
		return CurrentWorld;
	}

	ProjectileType CurrentPoolType = ProjectileType::Invalid;

private:
	UWorld* CurrentWorld = nullptr;
};
