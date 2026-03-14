// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FocusOnPlayer.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_FocusOnPlayer::UBTTask_FocusOnPlayer()
{
	NodeName = TEXT("Focus On Player");
}

EBTNodeResult::Type UBTTask_FocusOnPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!IsValid(ControlledPawn))
	{
		return EBTNodeResult::Failed;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(ControlledPawn, 0);
	if (!IsValid(PlayerPawn))
	{
		return EBTNodeResult::Failed;
	}

	const float DistanceToPlayer = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());

	if (DistanceToPlayer <= FocusDistance)
	{
		AIController->SetFocus(PlayerPawn);
	}
	else
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}

	return EBTNodeResult::Succeeded;
}
