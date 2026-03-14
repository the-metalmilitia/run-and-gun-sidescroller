// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraPlayer.h"

#include "Components/CapsuleComponent.h"
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
		}
	}
}

// Called every frame
void AContraPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsJumping)
	{
		if (GetWorld()->GetTimeSeconds() - CurrentJumpTime >= MidAirTimeBeforePlatformSwitch)
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
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AContraPlayer::ShootEvent);
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
	if (!IsAlive_Implementation() || DamageCauser == this)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);

	if (!IsAlive_Implementation())
	{
		PlayerController->CallPlayerDeath();
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AContraPlayer::IsAlive_Implementation() const
{
	return CurrentHealth > 0.0f;
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

