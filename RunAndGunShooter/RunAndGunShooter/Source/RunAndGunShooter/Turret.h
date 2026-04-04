// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DamageableInterface.h"
#include "TurretGun.h"
#include "ContraPlayer.h"
class UParticleSystem;

#include "Turret.generated.h"

class UBehaviorTree;

UCLASS()
class RUNANDGUNSHOOTER_API ATurret : public APawn, public IDamageableInterface
{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActivateTurret(bool playerDetected);

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* DamageCollider;

	UPROPERTY(VisibleAnywhere)
	ATurretGun* CurrentWeapon = nullptr;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Turret Properties")
	TSubclassOf<ATurretGun> GunClass;

	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float FireRate;
	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float DetectionRange;
	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float Health = 5;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* DeathVFX = nullptr;

	void FireShot();

	APawn* PlayerPawn = nullptr;
	FTimerHandle FireTimerHandle;

	void Die();
};
