// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "ProjectileDataManager.h"
#include "DamageableInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ProjectileMesh->SetupAttachment(Root);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionMesh");
	CollisionSphere->SetupAttachment(Root);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->Activate(false);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnBeginOverlap);
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

	if (activate && TrailVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, TrailVFX,
			GetActorLocation(), GetActorRotation());
	}
}

void AProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor, UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->Implements<UDamageableInterface>() && OtherActor != this)
	{
		IDamageableInterface::Execute_ApplyDamage(OtherActor, Damage, GetOwner());

		if (ImpactVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX,
				GetActorLocation(), FRotator::ZeroRotator);
		}
    }
}

