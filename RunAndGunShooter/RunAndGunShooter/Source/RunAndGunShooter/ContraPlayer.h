// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "ContraPlayer.generated.h"

class AContraPlayerController;

UENUM(BlueprintType)
enum VerticalSwitchOption
{
	None,
	Up,
	Down,
	Bothways
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

	UPROPERTY(EditDefaultsOnly)
	float MidAirTimeBeforePlatformSwitch = 0.5f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Score")
	float DistanceScoreInterval = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Score")
	int32 DistanceScoreAmount = 10;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShootMontage;

	UPROPERTY(VisibleAnywhere, Category = "Lives")
	float RespawnDropHeight = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = "Lives")
	float RespawnDepth = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Lives")
	float RespawnInvincibilityDuration = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Lives")
	float BlinkInterval = 0.1f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AllowPlatformSwitch(const VerticalSwitchOption allowedSwitch);

	UFUNCTION(BlueprintCallable)
	void SetRespawnDepth(float NewDepth) { RespawnDepth = NewDepth; }
	UFUNCTION(BlueprintCallable)
	void SetRespawnHeight(float NewHeight) { RespawnDropHeight = NewHeight; }

	UFUNCTION(BlueprintCallable)
	AGunBase* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintPure)
	bool HasCurrentWeapon() const { return IsValid(CurrentWeapon) && CurrentWeapon->Implements<UShooterInterface>(); }
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const { return CurrentHealth <= 0.0f; }

	void ApplyDamage(float DamageAmount, AActor* DamageCauser);
	bool IsAlive() const;

	void Shoot();
	

private:
	void MoveEvent(const FInputActionValue& Value);
	void VerticalMoveEvent(const FInputActionValue& Value);
	void JumpEvent(const FInputActionValue& Value);
	void ShootEvent(const FInputActionValue& Value);
	void LookEvent(const FInputActionValue& value);

	void Die();
	void Respawn();
	void SwitchPlatform();

	FVector RespawnLocation;

	FTimerHandle InvincibilityTimerHandle;
	FTimerHandle BlinkTimerHandle;
	bool bIsInvincible = false;

	VerticalSwitchOption VerticalSwitch = VerticalSwitchOption::None;
	VerticalSwitchOption AllowedSwitch = VerticalSwitchOption::None;

	AContraPlayerController* PlayerController = nullptr;

	UPROPERTY(VisibleAnywhere)
	AGunBase* CurrentWeapon = nullptr;

	bool bIsJumping = false;
	bool bIsPlatformSwitchAllowed = true;
	float CurrentJumpTime = 0.0f;

    FVector LastTickLocation = FVector::ZeroVector;
	FVector CurrentLocation = FVector::ZeroVector;
};
