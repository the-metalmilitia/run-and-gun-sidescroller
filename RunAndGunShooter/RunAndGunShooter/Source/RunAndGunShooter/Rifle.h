// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class RUNANDGUNSHOOTER_API ARifle : public AGunBase
{
	GENERATED_BODY()

public:
	virtual void Shoot_Implementation() override;

	
};
