#include "Projectile.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Engine/EngineTypes.h"
#include "Damageable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCollider"));
	CollisionMesh->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = CollisionMesh;
	SetProjectileEnabled(true);
}

void AProjectile::SetProjectileEnabled(const bool bNewEnabled)
{
	bEnabled = bNewEnabled;

	if (!CollisionMesh)
	{
		return;
	}

	CollisionMesh->SetVisibility(bEnabled);
	CollisionMesh->SetSimulatePhysics(bEnabled);
	
	if (bEnabled)
	{
		CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AProjectile::SetupProjectile(AEnemy* Enemy, const FProjectileValues& InProjectileValues)
{
	TargetEnemy = Enemy;
	ProjectileValues = InProjectileValues;
	ProjectileLifetimeTimer = ProjectileValues.Lifetime;
	SetActorScale3D(FVector(ProjectileValues.Scale, ProjectileValues.Scale, ProjectileValues.Scale));
	SetProjectileEnabled(true);
	CollisionMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * ProjectileValues.Speed, false);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (const UPhysicsSettings* Physics = UPhysicsSettings::Get())
	{
		Gravity = -Physics->DefaultGravityZ;
	}
}

void AProjectile::Tick(const float DeltaTime)
{
	if (!bEnabled)
	{
		return;
	}

	// TODO: make this a timer, rather than being in tick
	ProjectileLifetimeTimer = ProjectileLifetimeTimer - DeltaTime;

	if (ProjectileLifetimeTimer <= 0 )
	{
		SetProjectileEnabled(false);
	}

	UpdateTargetDest(DeltaTime);
}

void AProjectile::UpdateTargetDest_Implementation(const float DeltaTime)
{
	// Base projectile has no implementation
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || !OtherComp)
	{
		return;
	}

	const FName OtherCompName = OtherComp->GetCollisionProfileName();
	if (OtherCompName != EnemyProfileName.Name)
	{
		SetProjectileEnabled(false);
		return;
	}

	IDamageable::Execute_TakeDamage(OtherActor, ProjectileValues.Damage);
	SetProjectileEnabled(false);
}

