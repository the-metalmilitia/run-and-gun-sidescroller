// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
#include "ProjectileDataManager.h"
#include "ShooterInterface.h"
class UParticleSystem;

#include "GunBase.generated.h"

UCLASS(Blueprintable)
class RUNANDGUNSHOOTER_API AGunBase : public AActor, public IShooterInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void SetProjectile_Implementation(ProjectileType Type, int AmountPerShot) override;

	UFUNCTION(BlueprintCallable)
	ProjectileType GetProjectileType() const { return SpawnedProjectileType; }

	virtual void Shoot_Implementation() override;
	virtual UProjectileDataManager* GetProjectileDataManager_Implementation() const override;

	USceneComponent* GetProjectileSpawnPoint() const { return ProjectileSpawnPoint; }
	void SnapProjectileSpawnPoint(USceneComponent* TargetComponent, FName SocketName);
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Pool")
	TObjectPtr<UProjectileDataManager> ProjectileDataManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ProjectileAmountPerShot = 1;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* MuzzleFlashVFX = nullptr;

protected:
	ProjectileType SpawnedProjectileType;
	TArray<AProjectileBase*> ProjectilesPerShot;

};
