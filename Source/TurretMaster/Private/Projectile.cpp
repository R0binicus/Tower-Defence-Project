#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Damageable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCollider"));
	CollisionMesh->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionMesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionMesh;
	MovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::InitializeProjectile(AActor* Target, const FProjectileValues& InProjectileValues)
{
	TargetActor = Target;

	ProjectileValues = InProjectileValues;

	SetLifeSpan(ProjectileValues.Lifetime);
	MovementComponent->Velocity = GetActorForwardVector() * ProjectileValues.Speed;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	UpdateTargetDest();
}

void AProjectile::UpdateTargetDest_Implementation()
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

