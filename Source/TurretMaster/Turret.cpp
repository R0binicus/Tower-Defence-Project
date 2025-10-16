#include "Turret.h"
#include "Damageable.h"

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

    BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPoint"));
    BulletSpawnPoint->SetupAttachment(RootComponent);

    MuzzleDirectionSocket = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleDirectionSocket"));
    MuzzleDirectionSocket->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
    TurretLocation = GetActorLocation();
    InitialRotation = GetActorRotation();

    ProjectileValues.Damage = ProjectileDamage;
    ProjectileValues.Speed = ProjectileSpeed;
    ProjectileValues.Lifetime = ProjectileLifetime;
    ProjectileValues.TurnMultiplier = ProjectileTurnMultiplier;

    World = GetWorld();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    CurrentClosestEnemy = GetClosestEnemy();

    UpdateTurretValues();

    RotateTowardsEnemy(DeltaTime);

    ShootTimer = ShootTimer - DeltaTime;

    if (CanShoot())
    {
        Shoot();
    }
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
    if (!OtherActor->ActorHasTag(EnemyTagName))
    {
        return;
    }

    EnemyRefArray.Remove(OtherActor);
}

AActor* ATurret::GetClosestEnemy()
{
    TObjectPtr<AActor> PotentialClosestEnemy = nullptr;
    float CurrentClosestDistance = 0.f;

    for (size_t i = 0; i < EnemyRefArray.Num(); i++)
    {
        if (!EnemyRefArray[i])
        {
            continue;
        }

        if (!PotentialClosestEnemy)
        {
            // What will happen if it doesn't implement IIDamagable??
            if (IDamageable::Execute_IsDead(EnemyRefArray[i]))
            {
                continue;
            }
            CurrentClosestDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretLocation);
            PotentialClosestEnemy = EnemyRefArray[i];
        }
        else
        {
            float EnemyDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretLocation);

            if (EnemyDistance >= CurrentClosestDistance)
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
    CurrentTurretRotation = GetActorRotation();

    if (!CurrentClosestEnemy)
    {
        return;
    }

    if (!MuzzleDirectionSocket)
    {
        return;
    }

    MuzzleForward = MuzzleDirectionSocket->GetForwardVector();

    TargetLocation = CurrentClosestEnemy->GetActorLocation();
    TryGetDirectionToEnemy(TargetLocation, TargetDirection);

    TargetDotProduct = FVector::DotProduct(MuzzleForward, TargetDirection);
}

bool ATurret::TryGetDirectionToEnemy(const FVector& EnemyPosition, FVector& DirectionOut)
{
    if (!MuzzleDirectionSocket)
    {
        DirectionOut = FVector::ZeroVector;
        return false;
    }

    DirectionOut = EnemyPosition - MuzzleDirectionSocket->GetComponentLocation();
    DirectionOut.Normalize();

    return true;
}

void ATurret::RotateTowardsEnemy(const float DeltaTime)
{
    if (!MuzzleDirectionSocket)
    {
        return;
    }

    float TurretDesiredYaw = FindDesiredYaw();
    float TurretDesiredPitch = FindDesiredPitch();

    FRotator DesiredRotation = FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);
    FRotator NewRotation = FMath::RInterpTo(CurrentTurretRotation, DesiredRotation, DeltaTime, TurretTurnSpeed);
    SetActorRotation(NewRotation);
}

float ATurret::FindDesiredYaw()
{
    float TurretDesiredYaw = InitialRotation.Yaw;

    // Reset to initial rotation if there is no closest enemy
    if (CurrentClosestEnemy)
    {
        float TurretCurrentYaw = CurrentTurretRotation.Yaw;
        float YawDegreesToEnemy = FMath::RadiansToDegrees(FMath::Acos(TargetDotProduct));
        FVector CrossProduct = FVector::CrossProduct(MuzzleForward, TargetDirection);
        float CrossProductSign = FMath::Sign(CrossProduct.Z);
        TurretDesiredYaw = TurretCurrentYaw + (YawDegreesToEnemy * CrossProductSign);
    }

    return TurretDesiredYaw;
}

float ATurret::FindDesiredPitch()
{
    float TurretDesiredPitch = InitialRotation.Pitch;

    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return TurretDesiredPitch;
    }

    // Prevent turret from aiming vertically  
    // if the vertical distance is too great
    if (TargetDotProduct >= GiveUpVerticalAimThreshold)
    {
        float TurretCurrentPitch = CurrentTurretRotation.Pitch;
        float TargetPitchDifference = FMath::RadiansToDegrees(TargetDirection.Z - MuzzleForward.Z);
        TurretDesiredPitch = TurretCurrentPitch + TargetPitchDifference;

        // Used to make the turret face the enemy, when it is only just
        // outside it's aiming bounds
        TurretDesiredPitch = FMath::Clamp(TurretDesiredPitch, AimVerticalLowerBound, AimVerticalUpperBound);
    }

    return TurretDesiredPitch;
}

bool ATurret::CanShoot()
{
    if (!CurrentClosestEnemy)
    {
        return false;
    }

    if (ShootTimer > 0.f)
    {
        return false;
    }

    if (TargetDotProduct >= FacingTargetThreshold)
    {
        return true;
    }

    return false;
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

    if (!BulletSpawnPoint)
    {
        return;
    }

    const FRotator SpawnRotation = BulletSpawnPoint->GetComponentRotation();
    const FVector SpawnLocation = BulletSpawnPoint->GetComponentLocation();

    TObjectPtr<AProjectile> Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    if (Projectile)
    {
        Projectile->InitializeProjectile(CurrentClosestEnemy, ProjectileValues);
    }
    ShootTimer = ShootCooldown;

}

