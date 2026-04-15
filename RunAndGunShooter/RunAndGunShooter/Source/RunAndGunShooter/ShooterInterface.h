// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileBase.h"
#include "ShooterInterface.generated.h"

class UProjectileDataManager;

UINTERFACE(MinimalAPI, Blueprintable)
class UShooterInterface : public UInterface
{
	GENERATED_BODY()
};

class RUNANDGUNSHOOTER_API IShooterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooter")
	void SetProjectile(int AmountPerShot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooter")
	void Shoot();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Shooter")
	UProjectileDataManager* GetProjectileDataManager() const;
};
