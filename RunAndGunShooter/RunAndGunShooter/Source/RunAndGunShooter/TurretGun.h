// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "TurretGun.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ATurretGun : public AGunBase
{
	GENERATED_BODY()

public:
	// Rotates the gun toward WorldTargetLocation, constrained to Yaw in the attach parent's local space.
	void AimToward(FVector WorldTargetLocation, float DeltaSeconds, float RotationSpeed);

	virtual void Shoot_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Turret Gun")
	FName MuzzleSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Turret Gun")
	FRotator AimOffset = FRotator::ZeroRotator;

	FVector LastKnownPlayerLocation = FVector::ZeroVector;
};
