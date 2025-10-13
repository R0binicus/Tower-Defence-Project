// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

    RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
    RangeSphere->SetCollisionProfileName(TEXT("EnemyDetection"));
    RangeSphere->SetGenerateOverlapEvents(true);
    RangeSphere->SetSphereRadius(TurretRange);
    RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlapBegin);
    RangeSphere->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnOverlapEnd);
    RootComponent = RangeSphere;

    MuzzleSocket = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSocket"));
    MuzzleSocket->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
    TurretLocation = GetActorLocation();

    World = GetWorld();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    CurrentClosestEnemy = GetClosestEnemy();

    UpdateTurretValues();

    RotateTowardsEnemy(DeltaTime);

    ShootCheck(DeltaTime);
}

void ATurret::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->ActorHasTag(EnemyTagName))
    {
        return;
    }

    EnemyRefArray.Add(OtherActor);
}

void ATurret::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor->ActorHasTag(EnemyTagName))
    {
        return;
    }

    EnemyRefArray.Remove(OtherActor);
}

AActor* ATurret::GetClosestEnemy()
{
    TObjectPtr<AActor> PotentialClosestEnemy = nullptr;
    float CurrentClosestDistance;

    for (size_t i = 0; i < EnemyRefArray.Num(); i++)
    {
        if (!EnemyRefArray[i])
        {
            continue;
        }

        if (!PotentialClosestEnemy)
        {
            CurrentClosestDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretLocation);
            PotentialClosestEnemy = EnemyRefArray[i];
        }
        else
        {
            float EnemyDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretLocation);

            if (EnemyDistance <= CurrentClosestDistance)
            {
                continue;
            }

            CurrentClosestDistance = EnemyDistance;
            PotentialClosestEnemy = EnemyRefArray[i];
        }
    }

    return PotentialClosestEnemy;
}

void ATurret::UpdateTurretValues()
{
    TurretForward = GetActorForwardVector();
    if (CurrentClosestEnemy)
    {
        ClosestEnemyLocation = CurrentClosestEnemy->GetActorLocation();
        ClosestEnemyDirection = GetDirectionToEnemy(ClosestEnemyLocation);
        ClosestEnemyDotProduct = GetNormalizedDotProduct(TurretForward, ClosestEnemyDirection);
    }
}

FVector ATurret::GetDirectionToEnemy(const FVector& EnemyPosition)
{
    FVector Direction = EnemyPosition - TurretLocation;
    Direction.Z = 0.f;

    return Direction;
}

float ATurret::GetNormalizedDotProduct(FVector VectorA, FVector VectorB)
{
    VectorA.Normalize();
    VectorB.Normalize();
    return FVector::DotProduct(VectorA, VectorB);
}

FVector ATurret::GetNormalizedCrossProduct(FVector VectorA, FVector VectorB)
{
    VectorA.Normalize();
    VectorB.Normalize();
    return FVector::CrossProduct(VectorA, VectorB);
}

void ATurret::RotateTowardsEnemy(const float& DeltaTime)
{
    if (!CurrentClosestEnemy)
    {
        return;
    }

    float TurretCurrentYaw = GetActorRotation().Yaw;
    float DegreesToEnemy = FMath::RadiansToDegrees(FMath::Acos(ClosestEnemyDotProduct));
    float CrossProductSign = GetNormalizedCrossProduct(TurretForward, ClosestEnemyDirection).GetSignVector().Z;
    float TurretDesiredYaw = TurretCurrentYaw + (DegreesToEnemy*CrossProductSign);

    float NewYawRotation = FMath::Lerp(TurretCurrentYaw, TurretDesiredYaw, DeltaTime * TurretTurnSpeed);
    SetActorRotation(FRotator(0, NewYawRotation, 0));
}

void ATurret::ShootCheck(const float& DeltaTime)
{
    ShootTimer = ShootTimer - DeltaTime;

    if (!CurrentClosestEnemy)
    {
        return;
    }

    if (ShootTimer <= 0.f && ClosestEnemyDotProduct >= FacingTargetThreshold)
    {
        Shoot();
    }
}

void ATurret::Shoot()
{
    if (!ProjectileClass)
    {
        return;
    }

    if (!World)
    {
        return;
    }

    if (!MuzzleSocket)
    {
        return;
    }

    const FRotator SpawnRotation = MuzzleSocket->GetComponentRotation();
    const FVector SpawnLocation = MuzzleSocket->GetComponentLocation();

    AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    ShootTimer = ShootCooldown;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Shoot")));
}

