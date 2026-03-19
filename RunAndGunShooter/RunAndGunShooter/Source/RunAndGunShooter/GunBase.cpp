// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

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
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	SetProjectile_Implementation(ProjectileType::Default, 1);
}

void AGunBase::SetProjectile_Implementation(ProjectileType Type, int AmountPerShot)
{
	ProjectileAmountPerShot = AmountPerShot;
	SpawnedProjectileType = Type;
	if(IsValid(ProjectileDataManager))
	{
		ProjectileDataManager->CreateProjectilePool(this, Type);
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
			Projectile->Activate(true);
			UE_LOG(LogTemp, Warning, TEXT("Projectile of type %d shot"), SpawnedProjectileType);
		}
	}
	
}

UProjectileDataManager* AGunBase::GetProjectileDataManager_Implementation() const
{
	return ProjectileDataManager;
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

