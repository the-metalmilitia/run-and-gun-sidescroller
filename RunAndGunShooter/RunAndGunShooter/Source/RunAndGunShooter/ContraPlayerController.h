// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraGameMode.h"
#include "DamageableInterface.h"

#include "HUDUserWidget.h"
#include "GameFramework/PlayerController.h"
#include "ContraPlayerController.generated.h"

class AContraPlayer;
class UNiagaraSystem;

UCLASS()
class RUNANDGUNSHOOTER_API AContraPlayerController : public APlayerController, public IDamageableInterface
{
	GENERATED_BODY()

public:
	void PlayerDeath();
    void PlayerRespawn();
    int GetPlayerLives() const { return GameMode ? GameMode->Lives : 0; }

	void BeginPlay() override;

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* ImpactVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* DeathVFX = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDUserWidget> HUDWidgetClass;

private:
	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

	UFUNCTION()
	void HandleLivesChanged(int32 NewLives);

	UHUDUserWidget* HUDWidget;

    AContraGameMode* GameMode = nullptr;
	AContraPlayer* ContraPlayer = nullptr;
};
