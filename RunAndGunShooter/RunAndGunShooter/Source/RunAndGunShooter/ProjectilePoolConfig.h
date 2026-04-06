// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileBase.h"
#include "ProjectilePoolConfig.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API UProjectilePoolConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Pool")
	TArray<TSubclassOf<AProjectileBase>> TypesOfProjectile;

	static constexpr int32 PoolSize = 20;
};
