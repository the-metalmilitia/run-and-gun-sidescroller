// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "ShooterInterface.h"
#include "ProjectileDataManager.h"
#include "SniperAIController.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ASniperAIController : public AEnemyAIController, public IShooterInterface
{
	GENERATED_BODY()

public:
	ASniperAIController();

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;

	virtual void SetProjectile_Implementation(ProjectileType Type, int AmountPerShot) override;
	virtual void Shoot_Implementation() override;
	virtual UProjectileDataManager* GetProjectileDataManager_Implementation() const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Shooter")
	UProjectileDataManager* ProjectileDataManager;

	ProjectileType SpawnedProjectileType = ProjectileType::Default;
};
