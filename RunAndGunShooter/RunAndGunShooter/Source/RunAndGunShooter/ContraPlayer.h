// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContraPlayerController.h"
#include "DamageableInterface.h"
#include "GunBase.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "ContraPlayer.generated.h"

UENUM(BlueprintType)
enum VerticalSwitchOption
{
	None,
	Up,
	Down,
	Bothways
};

UCLASS(Blueprintable)
class RUNANDGUNSHOOTER_API AContraPlayer : public ACharacter, public IDamageableInterface
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

	UPROPERTY(EditDefaultsOnly)
	float MidAirTimeBeforePlatformSwitch = 0.5f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShootMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AllowPlatformSwitch(const VerticalSwitchOption allowedSwitch);

	UFUNCTION(BlueprintCallable)
	void TriggerPlayerDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	AGunBase* GetCurrentWeapon() const { return CurrentWeapon; }
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const { return CurrentHealth <= 0.0f; }

	virtual void ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;
	virtual bool IsAlive_Implementation() const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();
	

private:
	void MoveEvent(const FInputActionValue& Value);
	void VerticalMoveEvent(const FInputActionValue& Value);
	void JumpEvent(const FInputActionValue& Value);
	void ShootEvent(const FInputActionValue& Value);
	void LookEvent(const FInputActionValue& value);

	void SwitchPlatform();

	VerticalSwitchOption VerticalSwitch = VerticalSwitchOption::None;
	VerticalSwitchOption AllowedSwitch = VerticalSwitchOption::Bothways;

	AContraPlayerController* PlayerController = nullptr;

	UPROPERTY(VisibleAnywhere)
	AGunBase* CurrentWeapon = nullptr;

	bool bIsJumping = false;
	bool bIsPlatformSwitchAllowed = true;
	float CurrentJumpTime = 0.0f;
};
