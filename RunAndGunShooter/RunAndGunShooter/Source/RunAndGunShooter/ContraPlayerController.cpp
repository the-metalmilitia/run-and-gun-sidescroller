// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraPlayerController.h"

void AContraPlayerController::CallPlayerDeath()
{
	
}

void AContraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDUserWidget>(this, HUDWidgetClass);

		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}
