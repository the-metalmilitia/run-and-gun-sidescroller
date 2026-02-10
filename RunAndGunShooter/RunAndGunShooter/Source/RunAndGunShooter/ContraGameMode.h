// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ContraGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API AContraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxLives = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 CurrentLives = 3;

	virtual void StartPlay() override;
};
