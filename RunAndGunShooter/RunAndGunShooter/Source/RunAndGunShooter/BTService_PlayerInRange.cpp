// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerInRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerInRange::UBTService_PlayerInRange()
{
	NodeName = TEXT("Player In Range");
	DetectionRadius = 1000.0f;
	// Optionally adjust TickInterval via editor or here:
	// Interval = 0.5f;
}

void UBTService_PlayerInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_PlayerInRange: No AIController"));
		return;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_PlayerInRange: Controller has no pawn"));
		return;
	}

	UWorld* World = AIPawn->GetWorld();
	if (!World)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_PlayerInRange: Player pawn not found"));
		// Ensure blackboard is cleared when no player found
		if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsBool(PlayerInRangeKey.SelectedKeyName, false);
		}
		return;
	}

	const float Distance = FVector::Dist(AIPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	const bool bInRange = Distance <= DetectionRadius;

	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsBool(PlayerInRangeKey.SelectedKeyName, bInRange);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_PlayerInRange: BlackboardComponent missing"));
	}
}

