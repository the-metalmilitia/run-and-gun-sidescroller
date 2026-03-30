// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraPlayer.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	GameMode = Cast<AContraGameMode>(GetWorld()->GetAuthGameMode());
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
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AContraPlayer::LookEvent);
	}
}

void AContraPlayer::AllowPlatformSwitch(const VerticalSwitchOption allowedSwitch)
{
	AllowedSwitch = allowedSwitch;
}

void AContraPlayer::TriggerPlayerDamage(float DamageAmount)
{

	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = UDamageType::StaticClass();
	TakeDamage(DamageAmount, DamageEvent, PlayerController, this);
}

float AContraPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ApplyDamage_Implementation(ActualDamage, DamageCauser);
	return ActualDamage;
}

void AContraPlayer::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive_Implementation() || DamageCauser == this || bIsInvincible)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	if (!IsAlive_Implementation())
	{
		Die();
	}
}

bool AContraPlayer::IsAlive_Implementation() const
{
	return CurrentHealth > 0.0f;
}

void AContraPlayer::Die()
{
	const bool bHasLivesRemaining = IsValid(GameMode) && GameMode->Lives > 0;

	if (bHasLivesRemaining && IsValid(PlayerController))
	{
		RespawnLocation = GetActorLocation();

		// Respawn path: destroy weapon only (its BeginPlay will recreate the pool)
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}

		GameMode->Lives--;
		Respawn();
	}
	else
	{
		// Final death path: full cleanup
		GetCharacterMovement()->DisableMovement();

		if (IsValid(CurrentWeapon))
		{
			UProjectileDataManager* PoolManager = IShooterInterface::Execute_GetProjectileDataManager(CurrentWeapon);
			if (IsValid(PoolManager))
			{
				PoolManager->ClearPools();
			}
			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}

		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				SubSystem->RemoveMappingContext(InputMappingContext);
			}

			PlayerController->bAutoManageActiveCameraTarget = false;
			PlayerController->CallPlayerDeath();
			DetachFromControllerPendingDestroy();
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (DeathVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, DeathVFX,
				GetActorLocation(), FRotator::ZeroRotator);
		}

		Destroy();
	}
}

void AContraPlayer::Respawn()
{
	SetActorLocation(RespawnLocation + FVector(0.0f, 0.0f, RespawnDropHeight));
	CurrentHealth = MaxHealth;

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
		UnCrouch();
		VerticalSwitch = VerticalSwitchOption::Up;
	}
	else if (movement < 0
		&& (AllowedSwitch == VerticalSwitchOption::Bothways
			|| AllowedSwitch == VerticalSwitchOption::Down))
	{
		VerticalSwitch = VerticalSwitchOption::Down;
		Crouch();
	}
	else
	{
		UnCrouch();
		VerticalSwitch = VerticalSwitchOption::None;
		UE_LOG(LogTemp, Warning, TEXT("%f"),movement);
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
	FVector CurrentLocation = GetActorLocation();
	if (VerticalSwitch == VerticalSwitchOption::Down)
	{
		CurrentLocation.Y += PlatformSwitchDepth;
		SetActorLocation(CurrentLocation);
	}
	else if(VerticalSwitch == VerticalSwitchOption::Up)
	{
		CurrentLocation.Y -= PlatformSwitchDepth;
		SetActorLocation(CurrentLocation);
	}
}

