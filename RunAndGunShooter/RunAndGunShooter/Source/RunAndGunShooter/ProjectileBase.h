// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Components/SphereComponent.h>
#include "ProjectileBase.generated.h"

class UProjectileDataManager;


UENUM(BlueprintType)
enum ProjectileType
{
	Invalid = -1,
	Default = 0,
	Spread = 1,
	MachineGun = 2,
	Spinning = 3,
    Homing = 4
};
UCLASS()
class RUNANDGUNSHOOTER_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate(bool activate);

	void SetActive(bool bActive) { bIsActive = bActive; }

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintPure, Category = "Projectile Pool")
	bool IsProjectileActive() const { return bIsActive; }

	UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
	void SetOwningPool(UProjectileDataManager* Pool) { OwningPool = Pool; }

	UFUNCTION(BlueprintPure, Category = "Projectile Pool")
	UProjectileDataManager* GetOwningPool() const { return OwningPool; }

	float GetProjectileSpeed() const { return ProjectileSpeed; }
	float GetDamage() const { return Damage; }

	ProjectileType GetProjectileType() const { return Type; }

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

protected:
	ProjectileType Type = ProjectileType::Default;

private:
	bool bIsActive = false;

	UPROPERTY()
	TObjectPtr<UProjectileDataManager> OwningPool;
};
