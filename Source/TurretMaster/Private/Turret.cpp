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
    ShootTimer = ShootTimer - DeltaTime;

    CurrentClosestEnemy = GetClosestEnemy();

    UpdateTurretValues();

    if (!CurrentClosestEnemy)
    {
        return;
    }

    const FVector TargetLocation = CurrentClosestEnemy->GetActorLocation();
    const FVector TargetDirection = GetDirectionToEnemy(TargetLocation, MuzzleBaseLocation);

    RotateTowardsTarget(DeltaTime, TargetLocation, TargetDirection);

    if (CanShoot())
    {
        Shoot(TargetLocation);
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

    if (MuzzleDirectionSocket)
    {
        MuzzleForward = MuzzleDirectionSocket->GetForwardVector();
        MuzzleBaseLocation = MuzzleDirectionSocket->GetComponentLocation();
    }

    if (BulletSpawnPoint)
    {
        BulletSpawnLocation = BulletSpawnPoint->GetComponentLocation();
    }
}

FVector ATurret::GetDirectionToEnemy(const FVector& EnemyPosition, const FVector& SourcePosition)
{
    FVector EnemyDirection = EnemyPosition - SourcePosition;
    EnemyDirection.Normalize();

    return EnemyDirection;
}

FVector ATurret::PredictEnemyLocation(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime)
{
    return EnemyPosition + (EnemyVelocity * ProjectileFlightTime);
}

void ATurret::RotateTowardsTarget(const float DeltaTime, const FVector& TargetPosition, const FVector& TargetDirection)
{
    if (!MuzzleDirectionSocket)
    {
        return;
    }

    DesiredTurretRotation = FindDesiredRotation(TargetPosition, TargetDirection);
    FRotator NewRotation = FMath::RInterpTo(CurrentTurretRotation, DesiredTurretRotation, DeltaTime, TurretTurnSpeed);
    SetActorRotation(NewRotation);
}

FRotator ATurret::FindDesiredRotation(const FVector& TargetPosition, const FVector& TargetDirection)
{
    float TurretDesiredYaw = FindDesiredYaw(TargetPosition, TargetDirection);
    float TurretDesiredPitch = FindDesiredPitch(TargetPosition, TargetDirection);

    return FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);
}

float ATurret::FindDesiredYaw(const FVector& TargetPosition, const FVector& TargetDirection)
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

float ATurret::FindDesiredPitch(const FVector& TargetPosition, const FVector& TargetDirection)
{
    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return InitialRotation.Pitch;
    }

    const float TargetDotProduct = FVector::DotProduct(MuzzleForward, TargetDirection);

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

    float DesiredAngleDotProduct = FVector::DotProduct(MuzzleForward, DesiredTurretRotation.Vector());

    if (DesiredAngleDotProduct < FacingTargetThreshold)
    {
        return false;
    }

    return true;
}

void ATurret::Shoot(const FVector& TargetPosition)
{
    if (!ProjectileClass)
    {
        return;
    }

    if (!World)
    {
        return;
    }

    FRotator SpawnRotation = DesiredTurretRotation;
    
    float Height = BulletSpawnLocation.Z - TargetPosition.Z;
    float AngleRad = FMath::DegreesToRadians(DesiredTurretRotation.Pitch);
    ProjectileValues.PredictedLifetime = CalculateProjectileLifetime(AngleRad, Height, Gravity, ProjectileValues.Speed);

    if (AllowLocationPrediction && CurrentClosestEnemy)
    {
        CalculateEnemyFutureLocationValues(TargetPosition, CurrentClosestEnemy->GetVelocity(), ProjectileValues.PredictedLifetime, SpawnRotation);
    }

    ShootTimer = ShootCooldown;
    TObjectPtr<AProjectile> Projectile = World->SpawnActor<AProjectile>(ProjectileClass, BulletSpawnLocation, SpawnRotation);
    if (!Projectile)
    {
        return;
    }

    Projectile->InitializeProjectile(CurrentClosestEnemy, ProjectileValues);
}

void ATurret::CalculateEnemyFutureLocationValues(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime, FRotator& OutDesiredRotation)
{
    FVector TargetPosition = PredictEnemyLocation(EnemyPosition, EnemyVelocity, ProjectileFlightTime);
    FVector TargetDirection = GetDirectionToEnemy(TargetPosition, MuzzleBaseLocation);

    OutDesiredRotation = FindDesiredRotation(TargetPosition, TargetDirection);

    // Equation inputs
    float Height = BulletSpawnLocation.Z - TargetPosition.Z;
    float AngleRad = FMath::DegreesToRadians(DesiredTurretRotation.Pitch);

    ProjectileValues.PredictedLifetime = CalculateProjectileLifetime(AngleRad, Height, Gravity, ProjectileValues.Speed);
}

float ATurret::CalculateProjectileLifetime(const float AngleRad, const float Height, const float InGravity, const float InitialVelocity)
{
    // Equation taken from: https://www.omnicalculator.com/physics/projectile-motion
    // NOTE: Equation values will not be correct if the player is above the turret muzzle

    // Equation input calculation
    const float AngleSin = FMath::Sin(AngleRad);
    
    // Time equation
    float PredictTime = (pow(InitialVelocity * AngleSin, 2)) + (2 * InGravity * Height);
    PredictTime = InitialVelocity * AngleSin + sqrt(PredictTime);
    PredictTime = PredictTime / InGravity;

    return PredictTime;
}

float ATurret::CalculateRequiredVelocity(const float AngleRad, const float Height, const float InGravity, const float FlatDistToEnemy)
{
    // Equation taken from: https://physics.stackexchange.com/questions/27992/solving-for-initial-velocity-required-to-launch-a-projectile-to-a-given-destinat

    // Velocity equation
    float RequiredVel = 0.5 * InGravity * FlatDistToEnemy * FlatDistToEnemy;
    RequiredVel = RequiredVel / (FlatDistToEnemy * FMath::Tan(AngleRad) + Height);
    RequiredVel = (1 / FMath::Cos(AngleRad)) * sqrt(RequiredVel);

    return RequiredVel;
}
