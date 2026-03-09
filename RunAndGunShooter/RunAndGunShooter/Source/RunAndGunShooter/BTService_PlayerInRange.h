// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PlayerInRange.generated.h"

/**
 * Service that checks distance to the player and sets a Blackboard boolean key (PlayerInRange).
 */
UCLASS(DisplayName = "BT Service - Player In Range")
class RUNANDGUNSHOOTER_API UBTService_PlayerInRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_PlayerInRange();

protected:
	// Runs each service tick and updates the blackboard key
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Detection radius in world units
	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius;

	// Blackboard key to set (boolean)
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PlayerInRangeKey;
};
