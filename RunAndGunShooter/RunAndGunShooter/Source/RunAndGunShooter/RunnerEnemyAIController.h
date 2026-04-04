// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "DamageableInterface.h"
#include "RunnerEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API ARunnerEnemyAIController : public AEnemyAIController, public IDamageableInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AttackPlayer() override;

private:
	void StartMovement();
	void Die();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float Health = 3.f;
};
