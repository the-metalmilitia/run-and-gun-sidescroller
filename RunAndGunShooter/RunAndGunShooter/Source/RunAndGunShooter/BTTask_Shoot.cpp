// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "ShooterInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Shoot: No AIController"));
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Shoot: Controller has no pawn"));
		return EBTNodeResult::Failed;
	}

	// Verify the pawn implements the shooter interface
	if (!Pawn->GetClass()->ImplementsInterface(UShooterInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Shoot: Pawn does not implement UShooterInterface"));
		return EBTNodeResult::Failed;
	}

	// Call the interface Shoot() — this dispatches to the BlueprintNativeEvent implementation (e.g. Shoot_Implementation)
	IShooterInterface::Execute_Shoot(Pawn);

	return EBTNodeResult::Succeeded;
}

