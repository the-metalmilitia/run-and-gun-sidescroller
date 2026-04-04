// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Allow runtime assignment of a behavior tree asset
	void SetBehaviorTree(UBehaviorTree* NewBehaviorTree);

protected:
	virtual void AttackPlayer();
	void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 200.0f;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree = nullptr;

	APawn* PlayerPawn = nullptr;
	

	void BeginPlay() override;

};
