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

    MuzzleSocket = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleSocket"));
    MuzzleSocket->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
    TurretLocation = GetActorLocation();
    InitialRotation = GetActorRotation();

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
    if (!CurrentClosestEnemy)
    {
        return;
    }

    if (!MuzzleSocket)
    {
        return;
    }

    MuzzleLocation = MuzzleSocket->GetComponentLocation();

    MuzzleForward = MuzzleSocket->GetForwardVector();
    MuzzleForward.Normalize();

    TargetLocation = CurrentClosestEnemy->GetActorLocation();
    TargetDirection = GetDirectionToEnemy(TargetLocation);

    TargetDirection2D = TargetDirection;
    // We need to discuss this and figure out what is going on here
    //TargetDirection2D.Z = 0.f;

    Target2DDotProduct = GetNormalizedDotProduct(MuzzleForward, TargetDirection2D);
}

FVector ATurret::GetDirectionToEnemy(const FVector& EnemyPosition)
{
    FVector Direction = EnemyPosition - MuzzleLocation;
    Direction.Normalize();

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

void ATurret::RotateTowardsEnemy(const float DeltaTime)
{
    if (!MuzzleSocket)
    {
        return;
    }

    float NewYawRotation = FindNewYawRotation(DeltaTime);
    float NewPitchRotation = FindNewPitchRotation(DeltaTime);
    SetActorRotation(FRotator(NewPitchRotation, NewYawRotation, InitialRotation.Roll));
}

float ATurret::FindNewYawRotation(const float DeltaTime)
{
    float TurretDesiredYaw = InitialRotation.Yaw;
    float TurretCurrentYaw = GetActorRotation().Yaw;

    // Reset to initial rotation if there is no closest enemy
    if (CurrentClosestEnemy)
    {
        float HorizontalDegreesToEnemy = FMath::RadiansToDegrees(FMath::Acos(Target2DDotProduct));
        float CrossProductSign = GetNormalizedCrossProduct(MuzzleForward, TargetDirection2D).GetSignVector().Z;
        TurretDesiredYaw = TurretCurrentYaw + (HorizontalDegreesToEnemy * CrossProductSign);
    }

    return FMath::Lerp(TurretCurrentYaw, TurretDesiredYaw, DeltaTime * TurretTurnSpeed);
}

float ATurret::FindNewPitchRotation(const float DeltaTime)
{
    float TurretDesiredPitch = InitialRotation.Pitch;
    float TurretCurrentPitch = GetActorRotation().Pitch;

    // Reset to initial rotation if there is no closest enemy
    // Also prevent turret aiming from freaking out if the enemy 
    // is too close or is behind the turret's muzzle
    if (CurrentClosestEnemy && GetNormalizedDotProduct(TargetDirection, GetActorForwardVector()) >= GiveUpVerticalAimThreshold)
    {
        // Get dot product, ignoring X and Y
        // Can't this be simplified??? it's basically just a float
        FVector MuzzleForwardVertical = FVector(TargetDirection.X, TargetDirection.Y, MuzzleForward.Z);

        float VerticalDegreesToEnemy = FMath::RadiansToDegrees(FMath::Acos(GetNormalizedDotProduct(MuzzleForwardVertical, TargetDirection)));
        float CrossProductSign = GetNormalizedCrossProduct(TargetDirection, MuzzleForward).GetSignVector().Y;
        TurretDesiredPitch = TurretCurrentPitch + (VerticalDegreesToEnemy * CrossProductSign);

        // Used to make the turret face the enemy, when it is only just
        // outside it's aiming bounds
        TurretDesiredPitch = FMath::Clamp(TurretDesiredPitch, AimVerticalLowerBound, AimVerticalUpperBound);
    }

    return FMath::Lerp(TurretCurrentPitch, TurretDesiredPitch, DeltaTime * TurretTurnSpeed);
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

    if (Target2DDotProduct >= FacingTargetThreshold)
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

    if (!MuzzleSocket)
    {
        return;
    }

    const FRotator SpawnRotation = MuzzleSocket->GetComponentRotation();

    TObjectPtr<AProjectile> Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, SpawnRotation);
    ShootTimer = ShootCooldown;
}

