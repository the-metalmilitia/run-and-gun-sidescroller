// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Components/SphereComponent.h>
#include "ProjectileBase.generated.h"

class UProjectileDataManager;
class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;
class USoundBase;


UENUM(BlueprintType)
enum ProjectileType : uint8
{
	Default = 0,
	Missile = 1,
	Spread = 2,
	MachineGun = 3,
	Spinning = 4,
	Homing = 5,
	Invalid = 255
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

	void Activate(bool activate, AActor* InInstigator = nullptr);

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

	UFUNCTION(BlueprintPure, Category = "Projectile")
	AActor* GetProjectileInstigator() const { return ProjectileInstigator; }

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

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UNiagaraComponent* TrailComponent;

	UPROPERTY(VisibleAnywhere, Category = "SFX")
	UAudioComponent* FlyingAudioComponent;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* TrailVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* FlyingSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* HitSound = nullptr;


protected:
	ProjectileType Type = ProjectileType::Default;

private:
	bool bIsActive = false;

	UPROPERTY()
	TObjectPtr<AActor> ProjectileInstigator;

	UPROPERTY()
	TObjectPtr<UProjectileDataManager> OwningPool;
};
