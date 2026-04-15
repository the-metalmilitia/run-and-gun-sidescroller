// Fill out your copyright notice in the Description page of Project Settings.

#include "SniperAIController.h"
#include "GameFramework/Character.h"

void ASniperAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (ACharacter* CharacterPawn = Cast<ACharacter>(GetPawn()))
	{
		if (CharacterPawn->bIsCrouched) return;
	}
	Super::ApplyDamage_Implementation(DamageAmount, DamageCauser);
}
