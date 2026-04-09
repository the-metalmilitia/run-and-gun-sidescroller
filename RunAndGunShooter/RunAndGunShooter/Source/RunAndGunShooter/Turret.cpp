// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "TurretAIController.h"
#include "EnemyAIController.h"
#include "ShooterInterface.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATurretAIController::StaticClass();

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(Root);

	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(Root);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController()))
	{
		EnemyController->SetBehaviorTree(BehaviorTreeAsset);
	}

	if (GunClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CurrentWeapon = GetWorld()->SpawnActor<ATurretGun>(GunClass, GetActorTransform(), SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			CurrentWeapon->SetGunOwnerActor(this);
		}
	}
}

void ATurret::ApplyDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	if (!IsAlive_Implementation() || DamageCauser == this)
	{
		return;
	}

	if (Cast<AContraPlayer>(DamageCauser))
	{
		UE_LOG(LogTemp, Warning, TEXT("Turret hit by player projectile!"));
	}

	Health = FMath::Max(0.0f, Health - DamageAmount);

	if (ImpactVFX)
	{
		FVector SpawnLocation = DamageCauser ? DamageCauser->GetActorLocation() : GetActorLocation();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactVFX, SpawnLocation, FRotator::ZeroRotator);
	}

	if (!IsAlive_Implementation())
	{
		Die();
	}
}

bool ATurret::IsAlive_Implementation() const
{
	return Health > 0.0f;
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
		ActivateTurret(DistanceToPlayer <= DetectionRange);
	}

	/*APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenPos;
	if (PC && UGameplayStatics::ProjectWorldToScreen(PC, GetActorLocation(), ScreenPos, false))
	{
		if (ScreenPos.X < 0.f)
		{
			Die();
		}
	}*/
}

void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATurret::ActivateTurret(bool playerDetected)
{
	if (!playerDetected || !PlayerPawn || !IsAlive_Implementation() || !CurrentWeapon)
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		return;
	}

	if (!GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret::FireShot, FireRate, true);
	}

	CurrentWeapon->AimToward(PlayerPawn->GetActorLocation(), GetWorld()->GetDeltaSeconds(), RotationSpeed);
}

void ATurret::FireShot()
{
	if (CurrentWeapon)
	{
		IShooterInterface::Execute_Shoot(CurrentWeapon);
	}
}

void ATurret::Die()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	if (DeathVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathVFX,
			GetActorLocation(), FRotator::ZeroRotator);
	}
}
