// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "ShooterInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ContraPlayer.h"

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

	AContraPlayer* Character = static_cast<AContraPlayer*>(AIController->GetCharacter());
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTASK_Shoot: Character not found"));
		return EBTNodeResult::Failed;
	}

	Character->Shoot();

	return EBTNodeResult::Succeeded;
}

