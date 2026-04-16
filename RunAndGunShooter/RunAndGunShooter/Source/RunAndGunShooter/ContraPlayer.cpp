// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraPlayer.h"
#include "ContraPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundBase.h"
// Sets default values
AContraPlayer::AContraPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AContraPlayer::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
	PlayerController = Cast<AContraPlayerController>(Controller);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
	}


	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);

	if(Rifle != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		AGunBase* newGun = GetWorld()->SpawnActor<AGunBase>(Rifle, SpawnParams);

		if(newGun)
		{
			FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
			newGun->AttachToComponent(GetMesh(), AttachmentRule, TEXT("WeaponSocket"));

			CurrentWeapon = newGun;
			CurrentWeapon->SetGunOwnerActor(this);
		}
	}

	LastTickLocation = GetActorLocation();
	CurrentLocation = GetActorLocation();
}

// Called every frame
void AContraPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsJumping)
	{
		if ((GetWorld()->GetTimeSeconds() - CurrentJumpTime >= MidAirTimeBeforePlatformSwitch)
			|| IsCrouched())
		{
			SwitchPlatform();
		}
	}

	if (bIsLerpingDepth)
	{
		FVector Loc = GetActorLocation();
		Loc.Y = FMath::FInterpTo(Loc.Y, TargetPlatformDepth, DeltaTime, PlatformSwitchLerpSpeed);
		SetActorLocation(Loc);
		if (FMath::IsNearlyEqual(Loc.Y, TargetPlatformDepth, 1.0f))
		{
			Loc.Y = TargetPlatformDepth;
			SetActorLocation(Loc);
			bIsLerpingDepth = false;
		}
	}
}

// Called to bind functionality to input
void AContraPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AContraPlayer::MoveEvent);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AContraPlayer::JumpEvent);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AContraPlayer::ShootEvent);
		EnhancedInputComponent->BindAction(VerticalMoveAction, ETriggerEvent::Triggered, this, &AContraPlayer::VerticalMoveEvent);
		EnhancedInputComponent->BindAction(VerticalMoveAction, ETriggerEvent::Completed, this, &AContraPlayer::VerticalMoveEvent);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AContraPlayer::LookEvent);
	}
}

void AContraPlayer::AllowPlatformSwitch(const VerticalSwitchOption allowedSwitch)
{
 	AllowedSwitch = allowedSwitch;
}

void AContraPlayer::ApplyDamage(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive() || DamageCauser == this || bIsInvincible)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	if (!IsAlive())
	{
		Die();
	}
}

bool AContraPlayer::IsAlive() const
{
	return CurrentHealth > 0.0f;
}

void AContraPlayer::PlayDeathSound()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

void AContraPlayer::Die()
{
	PlayDeathSound();

	const bool bHasLivesRemaining = IsValid(PlayerController) && PlayerController->GetPlayerLives() > 0;

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	UnCrouch();
	bIsJumping = false;
	if (IsPlayerControlled() && IsValid(PlayerController))
	{
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->DestroyWeapon(!bHasLivesRemaining);
		}
		if (bHasLivesRemaining)
		{
			if (IsValid(CurrentWeapon))
			{
				CurrentWeapon->DestroyWeapon(false);
			}

			RespawnLocation = GetActorLocation();
			PlayerController->PlayerRespawn();

			Respawn();
		}
		else
		{
			PlayerController->PlayerDeath();
			
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				SubSystem->RemoveMappingContext(InputMappingContext);
			}

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Destroy();
		}
	}
	else
	{
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->DestroyWeapon(true);
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy();
	}

}

void AContraPlayer::Respawn()
{
	VerticalSwitch = VerticalSwitchOption::None;
	AllowedSwitch = VerticalSwitchOption::None;
	bIsLerpingDepth = false;

	RespawnLocation.Y = RespawnDepth;
	RespawnLocation.Z = RespawnDropHeight;
	SetActorLocation(RespawnLocation);
	CurrentHealth = MaxHealth;
	GetCharacterMovement()->SetComponentTickEnabled(true);
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Walking;
	GetCharacterMovement()->DisableMovement(); // Temporarily to reset
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	EnableInput(PlayerController);

	bIsInvincible = true;
	GetWorldTimerManager().SetTimer(InvincibilityTimerHandle,
		[this]()
		{
			bIsInvincible = false;
			GetWorldTimerManager().ClearTimer(BlinkTimerHandle);
			GetMesh()->SetVisibility(true);
		},
		RespawnInvincibilityDuration, false);

	GetWorldTimerManager().SetTimer(BlinkTimerHandle,
		[this]() { GetMesh()->SetVisibility(!GetMesh()->IsVisible()); },
		BlinkInterval, /*bLoop=*/true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (Rifle != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		AGunBase* newGun = GetWorld()->SpawnActor<AGunBase>(Rifle, SpawnParams);

		if (newGun)
		{
			FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
			newGun->AttachToComponent(GetMesh(), AttachmentRule, TEXT("WeaponSocket"));
			CurrentWeapon = newGun;
			CurrentWeapon->SetGunOwnerActor(this);
		}
	}
}

void AContraPlayer::MoveEvent(const FInputActionValue& Value)
{
	if (IsCrouched())
	{
		return;
	}
	
	float movement = Value.Get<float>();
	AddMovementInput(FVector::ForwardVector, movement * Speed * GetWorld()->GetDeltaSeconds());
	
	FRotator currentRotation = GetActorRotation();
	if (movement < 0 && currentRotation.Yaw != 180.0f)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	else if (movement > 0 && currentRotation.Yaw != 0.0f)
	{
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

void AContraPlayer::VerticalMoveEvent(const FInputActionValue& Value)
{
 	float movement = Value.Get<float>();
	if (movement > 0
		&& (AllowedSwitch == VerticalSwitchOption::Bothways
			|| AllowedSwitch == VerticalSwitchOption::Up))
	{
		Super::UnCrouch();
		UnCrouch();
		VerticalSwitch = VerticalSwitchOption::Up;
	}
	else if (movement < 0
		&& (AllowedSwitch == VerticalSwitchOption::Bothways
			|| AllowedSwitch == VerticalSwitchOption::Down))
	{
		VerticalSwitch = VerticalSwitchOption::Down;
		Super::Crouch();
		Crouch();
	}
	else
	{
		Super::UnCrouch();
		UnCrouch();
		VerticalSwitch = VerticalSwitchOption::None;
	}
}

void AContraPlayer::JumpEvent(const FInputActionValue& Value)
{
	Jump();
	if (bIsPlatformSwitchAllowed)
	{
		bIsJumping = true;
		CurrentJumpTime = GetWorld()->GetTimeSeconds();
	}
}

void AContraPlayer::ShootEvent(const FInputActionValue& Value)
{
	Shoot();
}

void AContraPlayer::Shoot()
{
	if (CurrentWeapon)
	{
		//CurrentWeapon->Shoot();
		IShooterInterface::Execute_Shoot(CurrentWeapon);
		if (ShootMontage && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(ShootMontage);
		}
	}
}

void AContraPlayer::LookEvent(const FInputActionValue& value)
{
	float lookAxis = value.Get<float>() * LookSpeed * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(-lookAxis);
}

void AContraPlayer::SwitchPlatform()
{
	bIsJumping = false;
	CurrentJumpTime = 0.0f;
	CurrentLocation = GetActorLocation();
	if (VerticalSwitch == VerticalSwitchOption::Down)
	{
		TargetPlatformDepth = CurrentLocation.Y + PlatformSwitchDepth;
		bIsLerpingDepth = true;
		UnCrouch();
		Jump();
	}
	else if (VerticalSwitch == VerticalSwitchOption::Up)
	{
		TargetPlatformDepth = CurrentLocation.Y - PlatformSwitchDepth;
		bIsLerpingDepth = true;
	}

	VerticalSwitch = VerticalSwitchOption::None;
}

