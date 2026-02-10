// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.h"
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
	void SetProjectile(ProjectileType type);

	UFUNCTION(BlueprintCallable)
	ProjectileType GetProjectileType() const { return SpawnedProjectileType; }

	virtual void Shoot();
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AProjectileBase>> TypesOfProjectile;

	UPROPERTY(EditAnywhere)
	int ProjectilePoolSize = 10;

private:
	ProjectileType SpawnedProjectileType;
	TQueue<AProjectileBase*> SpawnedProjectilePool;
};
