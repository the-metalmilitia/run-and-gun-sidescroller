// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "ShooterInterface.h"
#include "ProjectileDataManager.h"
#include "DamageableInterface.h"
#include "SniperAIController.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ASniperAIController : public AEnemyAIController, public IShooterInterface, public IDamageableInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetProjectile_Implementation(ProjectileType Type, int AmountPerShot) override;
	virtual void Shoot_Implementation() override;
	virtual UProjectileDataManager* GetProjectileDataManager_Implementation() const override;

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

private:
	void Die();

	UPROPERTY(EditDefaultsOnly, Category = "Shooter")
	TObjectPtr<UProjectileDataManager> ProjectileDataManager;

	ProjectileType SpawnedProjectileType = ProjectileType::Default;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float Health = 3.f;
};
