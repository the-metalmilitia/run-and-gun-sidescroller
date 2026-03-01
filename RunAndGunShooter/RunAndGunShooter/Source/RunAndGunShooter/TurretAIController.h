// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API ATurretAIController : public AEnemyAIController
{
	GENERATED_BODY()

protected:
	virtual void AttackPlayer() override;
	
};
