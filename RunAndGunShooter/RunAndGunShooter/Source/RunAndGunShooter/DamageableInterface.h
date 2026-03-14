// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class RUNANDGUNSHOOTER_API IDamageableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damageable")
	void ApplyDamage(float DamageAmount, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damageable")
	bool IsAlive() const;
};
