// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void AttackPlayer();

private:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200.0f;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree = nullptr;

	APawn* PlayerPawn = nullptr;
	

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

};
