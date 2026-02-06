// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
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

UCLASS(Blueprintable)
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
	UInputAction* VerticalMoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	float Speed = 600.0f;

	UPROPERTY(EditAnywhere)
	float LookSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float PlatformSwitchDepth = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGunBase> Rifle;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveEvent(const FInputActionValue& Value);
	void VerticalMoveEvent(const FInputActionValue& Value);
	void JumpEvent(const FInputActionValue& Value);
	void ShootEvent(const FInputActionValue& Value);
	void LookEvent(const FInputActionValue& value);

	VerticalSwitchOption::Enum VerticalSwitch = VerticalSwitchOption::Enum::None;

	APlayerController* PlayerController = nullptr;

	UPROPERTY(VisibleAnywhere)
	AGunBase* CurrentWeapon = nullptr;
};
