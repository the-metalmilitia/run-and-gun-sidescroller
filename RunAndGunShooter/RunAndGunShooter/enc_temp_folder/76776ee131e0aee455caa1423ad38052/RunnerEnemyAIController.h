// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "RunnerEnemyAIController.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ARunnerEnemyAIController : public AEnemyAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void StartMovement();

	FTimerHandle MovementCheckTimer;
};
