// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "ProjectileDataManager.h"
#include "DamageableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"


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

	TrailComponent = CreateDefaultSubobject<UNiagaraComponent>("TrailComponent");
	TrailComponent->SetupAttachment(Root);
	TrailComponent->bAutoActivate = false;

	FlyingAudioComponent = CreateDefaultSubobject<UAudioComponent>("FlyingAudioComponent");
	FlyingAudioComponent->SetupAttachment(Root);
	FlyingAudioComponent->bAutoActivate = false;
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

void AProjectileBase::Activate(bool activate, AActor* InInstigator)
{
	ProjectileInstigator = activate ? InInstigator : nullptr;
	bIsActive = activate;
	MovementComponent->Activate(activate);
	SetActorEnableCollision(activate);
	SetActorHiddenInGame(!activate);
	SetActorTickEnabled(activate);
	MovementComponent->Velocity = activate ? GetActorForwardVector() * ProjectileSpeed : FVector::ZeroVector;

	if (TrailComponent)
	{
		if (activate && TrailVFX)
		{
			TrailComponent->SetAsset(TrailVFX);
			TrailComponent->Activate(true);
		}
		else
		{
			TrailComponent->Deactivate();
		}
	}

	if (FlyingAudioComponent)
	{
		if (activate && FlyingSound)
		{
			FlyingAudioComponent->SetSound(FlyingSound);
			FlyingAudioComponent->Play();
		}
		else
		{
			FlyingAudioComponent->Stop();
		}
	}
}

void AProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == ProjectileInstigator) return;

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		// Controller owns damage for all pawns (player, Runner, Sniper)
		if (AController* Controller = Pawn->GetController())
		{
			if (Controller->Implements<UDamageableInterface>())
			{
				IDamageableInterface::Execute_ApplyDamage(Controller, Damage, GetProjectileInstigator());
				return;
			}
		}

		// Fallback: pawn manages its own damage (ATurret)
		if (OtherActor->Implements<UDamageableInterface>())
		{
			IDamageableInterface::Execute_ApplyDamage(OtherActor, Damage, GetProjectileInstigator());
		}
	}
}

