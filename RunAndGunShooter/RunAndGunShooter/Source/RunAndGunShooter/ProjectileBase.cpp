// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "ProjectileDataManager.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ProjectileMesh->SetupAttachment(Root);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->Activate(false);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::Activate(bool activate)
{
	bIsActive = activate;
	MovementComponent->Activate(activate);
	SetActorEnableCollision(activate);
	SetActorHiddenInGame(!activate);
	SetActorTickEnabled(activate);
	MovementComponent->Velocity = activate ? GetActorForwardVector() * ProjectileSpeed : FVector::ZeroVector;
}

