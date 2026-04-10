// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include <Kismet\GameplayStatics.h>

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if(!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn not found!"));
		return;
	}

	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree not assigned!"));
		return;
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PlayerPawn && PlayerPawn->GetDistanceTo(GetPawn()))
	{
		if (LineOfSightTo(PlayerPawn))
		{
			SetFocus(PlayerPawn);
		}
	}
	else
	{
		ClearFocus(EAIFocusPriority::Default);
	}
}

void AEnemyAIController::AttackPlayer()
{
}

void AEnemyAIController::SetBehaviorTree(UBehaviorTree* NewBehaviorTree)
{
	BehaviorTree = NewBehaviorTree;
	// If the controller is already running in the world and a BehaviorTree was provided, run it immediately
	if (BehaviorTree && GetWorld())
	{
		RunBehaviorTree(BehaviorTree);
	}
}
