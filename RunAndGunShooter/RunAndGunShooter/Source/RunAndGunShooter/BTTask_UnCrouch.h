// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UnCrouch.generated.h"

/**
 * Behavior Tree Task to make the AI Character stand up from a crouch.
 */
UCLASS(DisplayName = "UnCrouch")
class RUNANDGUNSHOOTER_API UBTTask_UnCrouch : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_UnCrouch();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
