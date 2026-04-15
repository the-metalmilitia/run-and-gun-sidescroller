// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"
#include "NiagaraComponent.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(Root);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileLaunchPoint");
	ProjectileSpawnPoint->SetupAttachment(Root);

	ProjectileDataManager = CreateDefaultSubobject<UProjectileDataManager>("ProjectileDataManager");

	MuzzleFlashComponent = CreateDefaultSubobject<UNiagaraComponent>("MuzzleFlashComponent");
	MuzzleFlashComponent->SetupAttachment(ProjectileSpawnPoint);
	MuzzleFlashComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	SetProjectile_Implementation(ProjectileAmountPerShot);

	if (MuzzleFlashComponent && MuzzleFlashVFX)
	{
		MuzzleFlashComponent->SetAsset(MuzzleFlashVFX);
	}
}

void AGunBase::SetProjectile_Implementation(int AmountPerShot)
{
	ProjectileAmountPerShot = AmountPerShot;
	if (IsValid(ProjectileDataManager))
	{
		ProjectileDataManager->CreateProjectilePool(this, SpawnedProjectileType);
	}
}

void AGunBase::Shoot_Implementation()
{
	ProjectileDataManager->GetProjectileFromPool(SpawnedProjectileType, ProjectilesPerShot, ProjectileAmountPerShot);
	for (AProjectileBase* Projectile : ProjectilesPerShot)
	{
		if (Projectile)
		{
			Projectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
			Projectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
			Projectile->SetOwningPool(ProjectileDataManager);
			Projectile->Activate(true, GunOwnerActor);
			UE_LOG(LogTemp, Warning, TEXT("Projectile of type %d shot"), SpawnedProjectileType);
		}
	}

	SpawnMuzzleVFX();
}

void AGunBase::SpawnMuzzleVFX()
{
	if (MuzzleFlashComponent)
	{
		MuzzleFlashComponent->Activate(true);
	}
}

UProjectileDataManager* AGunBase::GetProjectileDataManager_Implementation() const
{
	return ProjectileDataManager;
}

void AGunBase::DestroyWeapon(bool clearPool)
{
	if (clearPool && IsValid(ProjectileDataManager))
	{
		ProjectileDataManager->ClearPools();
	}

	Destroy();
}

void AGunBase::SnapProjectileSpawnPoint(USceneComponent* TargetComponent, FName SocketName)
{
	if (ProjectileSpawnPoint && TargetComponent)
	{
		ProjectileSpawnPoint->AttachToComponent(TargetComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName);
	}
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

