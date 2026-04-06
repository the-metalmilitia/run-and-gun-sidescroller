// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretGun.h"

void ATurretGun::BeginPlay()
{
	Super::BeginPlay();

	if (!MuzzleSocketName.IsNone())
	{
		SnapProjectileSpawnPoint(GunMesh, MuzzleSocketName);
	}
}

void ATurretGun::Shoot_Implementation()
{
	if (!ProjectileDataManager) return;

	ProjectileDataManager->GetProjectileFromPool(SpawnedProjectileType, ProjectilesPerShot, ProjectileAmountPerShot);

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
    FVector Direction = (LastKnownPlayerLocation - SpawnLocation).GetSafeNormal();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

	for (AProjectileBase* Projectile : ProjectilesPerShot)
	{
		if (Projectile)
		{
			Projectile->SetActorLocation(SpawnLocation);
			Projectile->SetActorRotation(SpawnRotation);
			Projectile->SetOwningPool(ProjectileDataManager);
			Projectile->Activate(true, GunOwnerActor);
		}
	}

	SpawnMuzzleVFX();
}

void ATurretGun::AimToward(FVector WorldTargetLocation, float DeltaSeconds, float RotationSpeed)
{
	LastKnownPlayerLocation = WorldTargetLocation;

	USceneComponent* SpawnPoint = GetProjectileSpawnPoint();
	if (!SpawnPoint) return;

	FVector Direction = SpawnPoint->GetComponentLocation() - WorldTargetLocation;

	// Work in the attach parent's local space to avoid world Euler axis remapping
	// caused by the turret actor being rotated (e.g. 90° on X).
	USceneComponent* AttachParent = GetRootComponent()->GetAttachParent();
	FVector LocalDir = AttachParent
		? AttachParent->GetComponentTransform().InverseTransformVectorNoScale(Direction.GetSafeNormal())
		: Direction.GetSafeNormal();

	FRotator LocalTarget = FRotationMatrix::MakeFromZ(LocalDir).Rotator() + AimOffset;
	FRotator LocalCurrent = GetRootComponent()->GetRelativeRotation();
	FRotator NewRot = FMath::RInterpTo(LocalCurrent, LocalTarget, DeltaSeconds, RotationSpeed);

	// Allow only Yaw — lock Pitch and Roll so the gun rotates like a clock in one plane.
	NewRot.Pitch = LocalCurrent.Pitch;
	NewRot.Roll = LocalCurrent.Roll;

	SetActorRelativeRotation(NewRot);
}
