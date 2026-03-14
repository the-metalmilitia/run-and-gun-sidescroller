// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FocusOnPlayer.generated.h"

/**
 * Focuses the AI controller on the player when within FocusDistance,
 * and clears focus when the player is outside that distance.
 */
UCLASS(DisplayName = "Focus On Player")
class RUNANDGUNSHOOTER_API UBTTask_FocusOnPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FocusOnPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Maximum distance at which the AI will focus on the player (cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Focus", meta = (ClampMin = "0.0"))
	float FocusDistance = 1500.0f;
};
