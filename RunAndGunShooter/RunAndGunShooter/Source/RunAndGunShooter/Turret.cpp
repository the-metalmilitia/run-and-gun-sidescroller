// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "TurretAIController.h"
#include "EnemyAIController.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ensure an AI Controller will be used for this pawn and that placed/spawned turrets get possessed
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATurretAIController::StaticClass();

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(Root);

	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(Root);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	SetProjectile_Implementation(ProjectileType::Default, 1);

	// If an EnemyAIController (or derived) is controlling this turret, pass the BehaviorTree asset to it
	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController()))
	{
		EnemyController->SetBehaviorTree(BehaviorTreeAsset);
	}
}

float ATurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn)
	{
		float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
		ActivateTurret(DistanceToPlayer <= DetectionRange);
	}
}

// Called to bind functionality to input
void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurret::ActivateTurret(bool playerDetected)
{
	if (!playerDetected || !PlayerPawn || !IsAlive())
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		return;
	}

	LastKnownPlayerLocation = PlayerPawn->GetActorLocation();

	if (!GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret::FireShot, FireRate, true);
	}

	DirectionToPlayer = ProjectileSpawnPoint->GetComponentLocation() - PlayerPawn->GetActorLocation();
	TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
	CurrentRotation = TurretMesh->GetComponentRotation();

	NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
    NewRotation.Yaw = CurrentRotation.Yaw;

	TurretMesh->SetWorldRotation(NewRotation);
}

void ATurret::SetProjectile_Implementation(ProjectileType Type, int AmountPerShot)
{
	SpawnedProjectileType = Type;
	if (ProjectileDataManager)
	{
		ProjectileDataManager->CreateProjectilePool(this, Type);
	}
}

void ATurret::Shoot_Implementation()
{
	if (!ProjectileDataManager || !IsAlive()) return;

	TArray<AProjectileBase*> Projectiles;
	ProjectileDataManager->GetProjectileFromPool(SpawnedProjectileType, Projectiles, 1);

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FVector Direction = (LastKnownPlayerLocation - SpawnLocation).GetSafeNormal();
	FRotator SpawnRotation = Direction.Rotation();

	for (AProjectileBase* Projectile : Projectiles)
	{
		if (Projectile)
		{
			Projectile->SetActorLocation(SpawnLocation);
			Projectile->SetActorRotation(SpawnRotation);
			Projectile->SetOwningPool(ProjectileDataManager);
			Projectile->Activate(true);
		}
	}
}

UProjectileDataManager* ATurret::GetProjectileDataManager_Implementation() const
{
	return ProjectileDataManager;
}

void ATurret::FireShot()
{
	Shoot_Implementation();
}

void ATurret::Die()
{
}
