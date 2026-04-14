// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "TurretAIController.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ATurretAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	// Delegate to ATurret so its health, VFX, and Die() logic are used
	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;
};
