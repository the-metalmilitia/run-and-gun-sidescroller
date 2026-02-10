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
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	SetProjectile(ProjectileType::Default);
}

void AGunBase::SetProjectile(ProjectileType type)
{
	SpawnedProjectileType = type;
	SpawnedProjectilePool.Empty();
	for(int i = 0; i < ProjectilePoolSize; i++)
	{
		SpawnedProjectilePool.Enqueue(GetWorld()->SpawnActor<AProjectileBase>(TypesOfProjectile[type]));
	}
}

void AGunBase::Shoot()
{
	AProjectileBase* Projectile = nullptr;
	SpawnedProjectilePool.Dequeue(Projectile);

	
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

