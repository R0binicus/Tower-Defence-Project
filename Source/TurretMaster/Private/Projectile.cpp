#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Damageable.h"

AProjectile::AProjectile()
{
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCollider"));
	CollisionMesh->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionMesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionMesh;
	MovementComponent->InitialSpeed = 3000.f;
	MovementComponent->MaxSpeed = 3000.f;
	MovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::SetProjectileValues(AActor* Target)
{
	TargetActor = Target;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(Lifetime);
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

	IDamageable::Execute_TakeDamage(OtherActor, Damage);
	Destroy();
}

