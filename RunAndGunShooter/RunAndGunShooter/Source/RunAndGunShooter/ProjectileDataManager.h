// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileBase.h"
#include "ProjectilePoolConfig.h"
#include "ProjectileDataManager.generated.h"

USTRUCT()
struct FProjectilePool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AProjectileBase*> Projectiles;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNANDGUNSHOOTER_API UProjectileDataManager : public UActorComponent
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

	UPROPERTY(EditAnywhere, Category = "Projectile Pool")
	TObjectPtr<UProjectilePoolConfig> Config;

private:
	UPROPERTY()
	TMap<int32, FProjectilePool> ProjectilePools;
};
