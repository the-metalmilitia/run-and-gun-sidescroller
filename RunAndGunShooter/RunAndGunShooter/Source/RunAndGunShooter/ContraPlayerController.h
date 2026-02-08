// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraGameMode.h"
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

public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TSubclassOf<UUserWidget> HUDClass;
//`
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TSubclassOf<UUserWidget> WinScreenClass;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TSubclassOf<UUserWidget> LoseScreenClass;
//
// private:
// 	TSubclassOf<AContraGameMode> GameModeClass;
};
