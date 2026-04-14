// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ContraGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLivesChanged, int32, NewLives);

UCLASS()
class RUNANDGUNSHOOTER_API AContraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lives")
	int32 Lives = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 Score = 0;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnLivesChanged OnLivesChanged;

	void AddScore(int32 Amount);
	void SetLives(int32 NewLives);
};
