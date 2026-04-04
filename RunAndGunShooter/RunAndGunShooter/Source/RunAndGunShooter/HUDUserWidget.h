// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

UCLASS()
class RUNANDGUNSHOOTER_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetLives(int32 Lives);

	UFUNCTION(BlueprintImplementableEvent)
	void SetScore(int32 Score);
};
