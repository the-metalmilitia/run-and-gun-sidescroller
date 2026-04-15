// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "SniperAIController.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ASniperAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
};
