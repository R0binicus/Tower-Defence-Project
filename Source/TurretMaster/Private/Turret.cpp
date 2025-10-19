#include "Turret.h"
#include "PhysicsEngine/PhysicsSettings.h"
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
    World = GetWorld();

    if (UPhysicsSettings* Physics = UPhysicsSettings::Get())
    {
        Gravity = -Physics->DefaultGravityZ;
    }

    ProjectileValues.Damage = ProjectileDamage;
    ProjectileValues.Speed = ProjectileSpeed;
    ProjectileValues.Lifetime = ProjectileLifetime;
    ProjectileValues.TurnMultiplier = ProjectileTurnMultiplier;
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
            if (!EnemyRefArray[i]->Implements<UDamageable>())
            {
                continue;
            }
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
    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return InitialRotation.Yaw;
    }

    // Make a 2D dot product, because we don't want the desired yaw to
    // worry about the pitch of the turret, or the pitch of the target
    FVector MuzzleForward2D = MuzzleForward;
    MuzzleForward2D.Z = 0;
    MuzzleForward2D.Normalize();
    FVector TargetDirection2D = TargetDirection;
    TargetDirection2D.Z = 0;
    TargetDirection2D.Normalize();

    const float DotProduct2D = FVector::DotProduct(MuzzleForward2D, TargetDirection2D);

    // Then calculate the degrees to enemy and it's sign
    const float YawDegreesToEnemy = FMath::RadiansToDegrees(FMath::Acos(DotProduct2D));
    const FVector CrossProduct = FVector::CrossProduct(MuzzleForward2D, TargetDirection2D);
    const float CrossProductSign = FMath::Sign(CrossProduct.Z);
    const float TurretCurrentYaw = CurrentTurretRotation.Yaw;

    return TurretCurrentYaw + (YawDegreesToEnemy * CrossProductSign);
}

float ATurret::FindDesiredPitch()
{
    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return InitialRotation.Pitch;
    }

    // Prevent turret from aiming vertically  
    // if the vertical distance is too great
    if (TargetDotProduct < GiveUpVerticalAimThreshold)
    {
        return InitialRotation.Pitch;
    }

    const float TurretCurrentPitch = CurrentTurretRotation.Pitch;
    const float TargetPitchDifference = FMath::RadiansToDegrees(TargetDirection.Z - MuzzleForward.Z);
    const float TurretDesiredPitch = TurretCurrentPitch + TargetPitchDifference;

    // Used to make the turret continue partially faceing the enemy, 
    // when the enmy is only just outside the turret's aiming bounds
    return FMath::Clamp(TurretDesiredPitch, AimVerticalLowerBound, AimVerticalUpperBound);
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
