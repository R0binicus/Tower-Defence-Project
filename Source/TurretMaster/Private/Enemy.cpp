#include "Enemy.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::TakeDamage_Implementation(float DamageTaken)
{
	CurrentHealth = CurrentHealth - DamageTaken;

	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0;
		bIsDead = true;
	}
}

void AEnemy::Death_Implementation()
{
	SetDestination(GetActorLocation());

	OnEnemyDeath.Broadcast(ResourcesOnKill);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::SetDestination(const FVector NewDestination)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}
	AIController->MoveToLocation(NewDestination, 5.f, false, true, true, false);
}

