#include "Projectile.h"
#include "Damageable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCollider"));
	CollisionMesh->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionMesh->SetSimulatePhysics(true);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = CollisionMesh;
}

void AProjectile::InitializeProjectile(AActor* Target, const FProjectileValues& InProjectileValues)
{
	TargetActor = Target;

	ProjectileValues = InProjectileValues;

	SetLifeSpan(ProjectileValues.Lifetime);
	CollisionMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * ProjectileValues.Speed, false);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
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
		Destroy();
		return;
	}

	IDamageable::Execute_TakeDamage(OtherActor, ProjectileValues.Damage);
	Destroy();
}

