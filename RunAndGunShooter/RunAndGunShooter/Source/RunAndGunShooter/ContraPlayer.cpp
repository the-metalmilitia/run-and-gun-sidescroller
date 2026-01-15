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
	}
}

void AContraPlayer::MoveEvent(const FInputActionValue& Value)
{
	FVector2D movement = Value.Get<FVector2D>();
	if (movement.X == 0)
	{
		VerticalSwitch = VerticalSwitchOption::Enum::None;
	}
	else
	{
		VerticalSwitch = movement.X > 0 ? VerticalSwitchOption::Enum::Up : VerticalSwitchOption::Enum::Down;
	}

	AddMovementInput(GetActorForwardVector(), movement.Y * Speed * GetWorld()->GetDeltaSeconds());

	if (movement.Y < 0)
	{
		AddControllerYawInput(-180.0f * GetWorld()->GetDeltaSeconds());
	}
}

void AContraPlayer::JumpEvent(const FInputActionValue& Value)
{
	Jump();
	if (VerticalSwitch == VerticalSwitchOption::Enum::None)
	{
		///change floor
	}
}

void AContraPlayer::ShootEvent(const FInputActionValue& Value)
{
	
}

