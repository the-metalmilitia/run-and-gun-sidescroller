// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraGameMode.h"
#include "HUDUserWidget.h"
#include "GameFramework/PlayerController.h"
#include "ContraPlayerController.generated.h"

/**
 *
 */
UCLASS()
class RUNANDGUNSHOOTER_API AContraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void CallPlayerDeath();

	void BeginPlay() override;

	UHUDUserWidget* GetHUDWidget() const { return HUDWidget; }

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDUserWidget> HUDWidgetClass;

private:
	UHUDUserWidget* HUDWidget;
};
