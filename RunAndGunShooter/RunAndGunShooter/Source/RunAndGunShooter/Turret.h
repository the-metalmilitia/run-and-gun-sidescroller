// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "DamageableInterface.h"
#include "ProjectileDataManager.h"
#include "ShooterInterface.h"
#include "Turret.generated.h"

class UBehaviorTree;

UCLASS()
class RUNANDGUNSHOOTER_API ATurret : public APawn, public IShooterInterface, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActivateTurret(bool playerDetected);

	virtual void SetProjectile_Implementation(ProjectileType Type, int AmountPerShot) override;
	virtual void Shoot_Implementation() override;
	virtual UProjectileDataManager* GetProjectileDataManager_Implementation() const override;

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* DamageCollider;
	UPROPERTY(VisibleAnywhere)
    USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Shooter")
	TObjectPtr<UProjectileDataManager> ProjectileDataManager;

	// AI: Behavior Tree asset that the turret will provide to its AI controller
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float FireRate;	
	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float DetectionRange;
	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Turret Properties")
	float Health = 5;

	void FireShot();

	APawn* PlayerPawn = nullptr;
	ProjectileType SpawnedProjectileType = ProjectileType::Default;
    FVector DirectionToPlayer;
    FVector LastKnownPlayerLocation = FVector::ZeroVector;
    FRotator TargetRotation;
    FRotator CurrentRotation;
    FRotator NewRotation;
    FTimerHandle FireTimerHandle;

	void Die();
};
