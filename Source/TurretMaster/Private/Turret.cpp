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

    TurretMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMeshComp"));
    TurretMeshComp->SetupAttachment(RootComponent);
    TurretMeshComp->SetRelativeRotation(FRotator(0, 270, 0));
}

void ATurret::SetProtectPoint_Implementation(AActor* NewProtectPoint)
{
    //TODO: Discuss, why doesn't this trigger???
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NewProtectPoint: %s"), *NewProtectPoint->GetName()));
    TurretProtectPoint = NewProtectPoint;
}

#pragma region Unreal Functions
// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
    TurretProtectPoint = this;
	Super::BeginPlay();
    TurretLocation = GetActorLocation();
    InitialRotation = GetActorRotation();
    World = GetWorld();

    if (const UPhysicsSettings* Physics = UPhysicsSettings::Get())
    {
        Gravity = -Physics->DefaultGravityZ;
    }

    ProjectileValues.Damage = ProjectileDamage;
    ProjectileValues.Speed = ProjectileSpeed;
    ProjectileValues.Lifetime = ProjectileLifetime;
    ProjectileValues.TurnMultiplier = ProjectileTurnMultiplier;

    MakeProjectiles(InitialProjectilePoolSize);

    UpdateTurretValues();

    TurretFireMinimumRange = FVector::DistSquared(MuzzleBaseLocation, BulletSpawnLocation) + pow(ExtraTurretFireMinimumRange, 2);
}

// Called every frame
void ATurret::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
    ShootTimer = ShootTimer - DeltaTime;

    CurrentClosestEnemy = GetClosestEnemy();

    UpdateTurretValues();

    FVector TargetLocation = FVector::ZeroVector;
    if (CurrentClosestEnemy)
    {
        TargetLocation = CurrentClosestEnemy->GetActorLocation();
    }
    
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
#pragma endregion Unreal Functions

#pragma region Projectile Pool
void ATurret::MakeProjectiles(const int NewProjectileAmount)
{
    ProjectilePool.Reserve(ProjectilePool.Num() + NewProjectileAmount);
    for (size_t i = 0; i < NewProjectileAmount; i++)
    {
        const TObjectPtr<AProjectile> Projectile = CreateProjectile();
        if (!Projectile)
        {
            return;
        }
    }
}

AProjectile* ATurret::CreateProjectile()
{
    if (!ensure(ProjectileClass))
    {
        return nullptr;
    }

    if (!World)
    {
        return nullptr;
    }

    const TObjectPtr<AProjectile> Projectile = World->SpawnActor<AProjectile>(ProjectileClass, TurretLocation, FRotator::ZeroRotator);
    if (!Projectile)
    {
        return nullptr;
    }

    Projectile->SetProjectileEnabled(false);
    ProjectilePool.Add(Projectile);

    return Projectile;
}

AProjectile* ATurret::GetUnusedProjectile() const
{
    for (size_t i = 0; i < ProjectilePool.Num(); i++)
    {
        if (!ProjectilePool[i])
        {
            continue;
        }

        if (!ProjectilePool[i]->IsProjectileEnabled())
        {
            return ProjectilePool[i];
        }
    }
    return nullptr;
}
#pragma endregion Projectile Pool

#pragma region Turret Update Values
AActor* ATurret::GetClosestEnemy() const
{
    TObjectPtr<AActor> PotentialClosestEnemy = nullptr;
    float CurrentClosestDistance = INFINITY;

    if (!TurretProtectPoint)
    {
        return nullptr;
    }

    for (size_t i = 0; i < EnemyRefArray.Num(); i++)
    {
        if (!EnemyRefArray[i])
        {
            continue;
        }

        if (!EnemyRefArray[i]->Implements<UDamageable>())
        {
            continue;
        }

        if (IDamageable::Execute_IsDead(EnemyRefArray[i]))
        {
            continue;
        }

        const float EnemyDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretProtectPoint->GetActorLocation());

        // TurretFireMinimumRadius is already squared, so comparing distances is fine
        if (EnemyDistance < TurretFireMinimumRange)
        {
            continue;
        }

        if (EnemyDistance >= CurrentClosestDistance)
        {
            continue;
        }

        CurrentClosestDistance = EnemyDistance;
        PotentialClosestEnemy = EnemyRefArray[i];
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

FVector ATurret::GetDirectionToEnemy(const FVector& EnemyPosition, const FVector& SourcePosition) const
{
    FVector EnemyDirection = EnemyPosition - SourcePosition;
    EnemyDirection.Normalize();

    return EnemyDirection;
}

FVector ATurret::PredictEnemyLocation(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime) const
{
    return EnemyPosition + (EnemyVelocity * ProjectileFlightTime);
}
#pragma endregion Turret Update Values

#pragma region Turret Rotation
void ATurret::RotateTowardsTarget(const float DeltaTime, const FVector& TargetPosition, const FVector& TargetDirection)
{
    if (!MuzzleDirectionSocket)
    {
        return;
    }

    DesiredTurretRotation = FindDesiredRotation(TargetPosition, TargetDirection);

    // Then clamp it if it is not allowed
    const float TurretDesiredPitch = FMath::Clamp(DesiredTurretRotation.Pitch, AimVerticalLowerBound, AimVerticalUpperBound);
    const FRotator ClampedTurretRotation = FRotator(TurretDesiredPitch, DesiredTurretRotation.Yaw, DesiredTurretRotation.Roll);

    // Then set rotation
    const FRotator NewRotation = FMath::RInterpTo(CurrentTurretRotation, ClampedTurretRotation, DeltaTime, TurretTurnSpeed);
    SetActorRotation(NewRotation);
}

FRotator ATurret::FindDesiredRotation(const FVector& TargetPosition, const FVector& TargetDirection)
{
    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return InitialRotation;
    }

    const float DesiredYaw = FindDesiredYaw(TargetPosition, TargetDirection);
    const float DesiredPitch = FindDesiredPitch(TargetPosition, TargetDirection);

    return FRotator(DesiredPitch, DesiredYaw, InitialRotation.Roll);
}

float ATurret::FindDesiredYaw(const FVector& TargetPosition, const FVector& TargetDirection) const
{
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
    // Prevent turret from aiming vertically  
    // if the vertical distance is too great
    const float TargetDotProduct = FVector::DotProduct(MuzzleForward, TargetDirection);
    if (TargetDotProduct < GiveUpVerticalAimThreshold)
    {
        return InitialRotation.Pitch;
    }

    const float TurretCurrentPitch = CurrentTurretRotation.Pitch;
    const float TargetPitchDifference = FMath::RadiansToDegrees(TargetDirection.Z - MuzzleForward.Z);
    const float TurretDesiredPitch = TurretCurrentPitch + TargetPitchDifference;

    return TurretDesiredPitch;
}
#pragma endregion Turret Rotation

#pragma region Shooting
bool ATurret::CanShoot() const
{
    if (!CurrentClosestEnemy)
    {
        return false;
    }

    if (ShootTimer > 0.f)
    {
        return false;
    }

    const float DesiredAngleDotProduct = FVector::DotProduct(MuzzleForward, DesiredTurretRotation.Vector());
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

    if (!CurrentClosestEnemy)
    {
        return;
    }

    ShootTimer = ShootCooldown;
    PreBulletSpawnSetValues(TargetPosition);
    FRotator SpawnRotation = DesiredTurretRotation;
    if (AllowLocationPrediction)
    {
        CalculateEnemyFutureLocationValues(TargetPosition, CurrentClosestEnemy->GetVelocity(), ProjectileValues.PredictedLifetime, SpawnRotation);
    }

    if (TurretShootAnimation && TurretMeshComp)
    {
        TurretMeshComp->PlayAnimation(TurretShootAnimation, false);
    }

    TObjectPtr<AProjectile> Projectile = GetUnusedProjectile();
    if (!Projectile)
    {
        Projectile = CreateProjectile();
        if (!Projectile)
        {
            return;
        }
    }

    Projectile->SetActorLocationAndRotation(BulletSpawnLocation, SpawnRotation);

    auto FireProjectileDelay = [this, Projectile]()
        {
            if (!Projectile)
            {
                return;
            }

            Projectile->SetupProjectile(CurrentClosestEnemy, ProjectileValues);

            // Reset ProjectileValues if custom projectile speed was changed (changed in ArcTurret)
            if (ProjectileValues.Speed != ProjectileSpeed)
            {
                ProjectileValues.Speed = ProjectileSpeed;
            }
        };

    GetWorldTimerManager().ClearTimer(ShootDelayHandle);
    ShootDelayHandle = GetWorldTimerManager().SetTimerForNextTick(FireProjectileDelay);
}

void ATurret::PreBulletSpawnSetValues(const FVector& TargetPosition)
{
    const float Height = BulletSpawnLocation.Z - TargetPosition.Z;
    const float AngleRad = FMath::DegreesToRadians(DesiredTurretRotation.Pitch);
    ProjectileValues.PredictedLifetime = CalculateProjectileLifetime(AngleRad, Height, Gravity, ProjectileValues.Speed);
}

void ATurret::CalculateEnemyFutureLocationValues(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime, FRotator& OutDesiredRotation)
{
    const FVector TargetPosition = PredictEnemyLocation(EnemyPosition, EnemyVelocity, ProjectileFlightTime);
    const FVector TargetDirection = GetDirectionToEnemy(TargetPosition, MuzzleBaseLocation);

    OutDesiredRotation = FindDesiredRotation(TargetPosition, TargetDirection);

    PreBulletSpawnSetValues(TargetPosition);
}
#pragma endregion Shooting

#pragma region Projectile Motion Calculations
float ATurret::CalculateProjectileLifetime(const float AngleRad, const float Height, const float InGravity, const float InitialVelocity) const
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

float ATurret::CalculateRequiredVelocity(const float AngleRad, const float Height, const float InGravity, const float FlatDistToEnemy) const
{
    // Equation taken from: https://physics.stackexchange.com/questions/27992/solving-for-initial-velocity-required-to-launch-a-projectile-to-a-given-destinat

    // Velocity equation
    float RequiredVel = 0.5 * InGravity * FlatDistToEnemy * FlatDistToEnemy;
    RequiredVel = RequiredVel / (FlatDistToEnemy * FMath::Tan(AngleRad) + Height);
    RequiredVel = (1 / FMath::Cos(AngleRad)) * sqrt(RequiredVel);

    return RequiredVel;
}
#pragma endregion Projectile Motion Calculations
