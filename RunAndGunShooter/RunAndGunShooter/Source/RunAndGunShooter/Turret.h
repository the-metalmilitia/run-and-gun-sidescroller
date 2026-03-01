// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API ATurret : public APawn
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
	void Shoot();

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
	float FireRate;	
	UPROPERTY(EditAnywhere)
	float DetectionRange;
	UPROPERTY(EditAnywhere)
	float RotationSpeed;


};
