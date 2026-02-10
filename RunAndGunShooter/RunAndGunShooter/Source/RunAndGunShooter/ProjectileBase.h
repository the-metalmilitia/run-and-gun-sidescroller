// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"


UENUM(BlueprintType)
enum ProjectileType
{
	Default = 0,
	Spread = 1,
	MachineGun = 2,
	Spinning = 3
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

	float GetProjectileSpeed() const { return ProjectileSpeed; }
	float GetDamage() const { return Damage; }

	ProjectileType GetProjectileType() const { return Type; }

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

protected:
	ProjectileType Type = ProjectileType::Default;
};
