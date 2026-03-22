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
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Shoot: No Pawn"));
		return EBTNodeResult::Failed;
	}

	// Prefer the pawn itself; fall back to any attached actor (e.g. GunBase).
	UObject* Shooter = nullptr;
	if (Pawn->Implements<UShooterInterface>())
	{
		Shooter = Pawn;
	}
	else
	{
		TArray<AActor*> AttachedActors;
		Pawn->GetAttachedActors(AttachedActors);
		for (AActor* Attached : AttachedActors)
		{
			if (Attached->Implements<UShooterInterface>())
			{
				Shooter = Attached;
				break;
			}
		}
	}

	if (!Shooter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Shoot: No IShooterInterface found on pawn or its attached actors"));
		return EBTNodeResult::Failed;
	}

	IShooterInterface::Execute_Shoot(Shooter);

	return EBTNodeResult::Succeeded;
}

