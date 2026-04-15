// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DamageableInterface.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UNiagaraSystem;

UCLASS()
class RUNANDGUNSHOOTER_API AEnemyAIController : public AAIController, public IDamageableInterface
{
	GENERATED_BODY()

public:
	void SetBehaviorTree(UBehaviorTree* NewBehaviorTree);

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

protected:
	virtual void BeginPlay() override;
	void Tick(float DeltaTime) override;
	virtual void Die();
	void DestroyPawn();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float Health = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	int32 ScoreValue = 10;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* ImpactVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* DeathVFX = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float ThresholdAlertDistance = 200.0f;

	APawn* PlayerPawn = nullptr;
	APawn* ControlledPawn = nullptr;
	APlayerController* ContraPlayerController = nullptr;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree = nullptr;

};
