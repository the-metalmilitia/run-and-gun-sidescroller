// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_UnCrouch.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_UnCrouch::UBTTask_UnCrouch()
{
	NodeName = TEXT("UnCrouch");
}

EBTNodeResult::Type UBTTask_UnCrouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
	if (!IsValid(Character))
	{
		return EBTNodeResult::Failed;
	}

	if (!Character->GetCharacterMovement()->IsCrouching())
	{
		return EBTNodeResult::Succeeded;
	}

	Character->UnCrouch();
	return EBTNodeResult::Succeeded;
}
