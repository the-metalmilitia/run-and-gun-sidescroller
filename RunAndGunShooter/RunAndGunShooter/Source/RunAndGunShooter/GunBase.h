// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
#include "ProjectileDataManager.h"
#include "GunBase.generated.h"

UCLASS(Blueprintable)
class RUNANDGUNSHOOTER_API AGunBase : public AActor
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
	UFUNCTION(BlueprintCallable)
	void SetProjectile(ProjectileType type, int AmountPerShot = 1);

	UFUNCTION(BlueprintCallable)
	ProjectileType GetProjectileType() const { return SpawnedProjectileType; }

	virtual void Shoot();
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileDataManager* ProjectileDataManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ProjectileAmountPerShot = 1;

protected:
	ProjectileType SpawnedProjectileType;
	TArray<AProjectileBase*> ProjectilesPerShot;

};
