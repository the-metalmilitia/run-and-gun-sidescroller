// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretAIController.h"
#include "Turret.h"

void ATurretAIController::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (ATurret* Turret = Cast<ATurret>(GetPawn()))
	{
		IDamageableInterface::Execute_ApplyDamage(Turret, DamageAmount, DamageCauser);
	}
}

bool ATurretAIController::IsAlive_Implementation() const
{
	if (const ATurret* Turret = Cast<ATurret>(GetPawn()))
	{
		return IDamageableInterface::Execute_IsAlive(Turret);
	}
	return false;
}
