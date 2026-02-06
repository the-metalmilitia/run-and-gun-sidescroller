// Fill out your copyright notice in the Description page of Project Settings.


#include "ContraPlayer.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
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
	
	PlayerController = Cast<APlayerController>(Controller);
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
	if (movement > 0)
	{
		UnCrouch();
		VerticalSwitch = VerticalSwitchOption::Enum::Up;
	}
	else if (movement < 0)
	{
		VerticalSwitch = VerticalSwitchOption::Enum::Down;
		Crouch();
	}
	else
	{
		UnCrouch();
		VerticalSwitch = VerticalSwitchOption::Enum::None;
		UE_LOG(LogTemp, Warning, TEXT("%f"),movement);
	}
}

void AContraPlayer::JumpEvent(const FInputActionValue& Value)
{
	Jump();

	FVector CurrentLocation = GetActorLocation();
	if (VerticalSwitch == VerticalSwitchOption::Enum::Down)
	{
		CurrentLocation.Y += PlatformSwitchDepth;
		SetActorLocation(CurrentLocation);
	}
	else if(VerticalSwitch == VerticalSwitchOption::Enum::Up)
	{
		CurrentLocation.Y -= PlatformSwitchDepth;
		SetActorLocation(CurrentLocation);
	}
}

void AContraPlayer::ShootEvent(const FInputActionValue& Value)
{
	
}

void AContraPlayer::LookEvent(const FInputActionValue& value)
{
	float lookAxis = value.Get<float>() * LookSpeed * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(-lookAxis);
}

