// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Crouch.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_Crouch::UBTTask_Crouch()
{
	NodeName = TEXT("Crouch");
}

EBTNodeResult::Type UBTTask_Crouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Crouch: No AIController"));
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Crouch: Controller has no pawn"));
		return EBTNodeResult::Failed;
	}

	ACharacter* Character = Cast<ACharacter>(Pawn);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_Crouch: Pawn is not ACharacter"));
		return EBTNodeResult::Failed;
	}

	// If already crouched, succeed immediately.
	if (Character->GetCharacterMovement() && Character->GetCharacterMovement()->IsCrouching())
	{
		return EBTNodeResult::Succeeded;
	}

	// Check whether character can crouch and then crouch.
	if (Character->CanCrouch())
	{
		Character->Crouch();
		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Warning, TEXT("UBTTask_Crouch: Character cannot crouch"));
	return EBTNodeResult::Failed;
}

