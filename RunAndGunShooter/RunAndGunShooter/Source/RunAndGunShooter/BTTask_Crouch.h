// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Crouch.generated.h"

/**
 * Behavior Tree Task to make the AI Character crouch.
 */
UCLASS(DisplayName = "Crouch")
class RUNANDGUNSHOOTER_API UBTTask_Crouch : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Crouch();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
