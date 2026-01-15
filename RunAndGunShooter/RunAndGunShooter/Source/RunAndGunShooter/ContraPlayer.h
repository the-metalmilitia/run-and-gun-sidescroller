// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "ContraPlayer.generated.h"

struct VerticalSwitchOption
{
	enum Enum
	{
		None,
		Up,
		Down
	};
};

UCLASS()
class RUNANDGUNSHOOTER_API AContraPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AContraPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere)
	float Speed = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float UpperPlatformSwitchDepth = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float LowerPlatformSwitchDepth = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveEvent(const FInputActionValue& Value);
	void JumpEvent(const FInputActionValue& Value);
	void ShootEvent(const FInputActionValue& Value);

	VerticalSwitchOption::Enum VerticalSwitch = VerticalSwitchOption::Enum::None;

	APlayerController* PlayerController = nullptr;
};
