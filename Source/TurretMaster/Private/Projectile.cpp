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

void AProjectile::SetProjectileEnabled(bool bNewEnabled)
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

void AProjectile::SetupProjectile(AActor* Target, const FProjectileValues& InProjectileValues)
{
	TargetActor = Target;
	ProjectileValues = InProjectileValues;
	ProjectileLifetimeTimer = ProjectileValues.Lifetime;

	SetProjectileEnabled(true);
	CollisionMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * ProjectileValues.Speed, false);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (UPhysicsSettings* Physics = UPhysicsSettings::Get())
	{
		Gravity = -Physics->DefaultGravityZ;
	}
}

void AProjectile::Tick(float DeltaTime)
{
	ProjectileLifetimeTimer = ProjectileLifetimeTimer - DeltaTime;

	if (ProjectileLifetimeTimer <= 0 && bEnabled)
	{
		SetProjectileEnabled(false);
	}

	if (!bEnabled)
	{
		return;
	}

	UpdateTargetDest(DeltaTime);
}

void AProjectile::UpdateTargetDest_Implementation(float DeltaTime)
{
	// Base projectile has no implementation
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr) return;
	if (OtherComp == nullptr) return;

	FName OtherCompName = OtherComp->GetCollisionProfileName();

	if (OtherCompName != "Pawn")
	{
		SetProjectileEnabled(false);
		return;
	}

	IDamageable::Execute_TakeDamage(OtherActor, ProjectileValues.Damage);
	SetProjectileEnabled(false);
}

