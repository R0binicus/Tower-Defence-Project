#include "Enemy.h"
#include "AIController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetupAttachment(RootComponent);
		HealthBarWidgetComponent->SetCastShadow(false);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		CameraManager = PlayerController->PlayerCameraManager;
	}

	if (HealthBarWidgetComponent)
	{
		HealthBarWidget = Cast<UEnemyHealthbarWidget>(HealthBarWidgetComponent->GetWidget());
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetBarPercent(CurrentHealth/CurrentHealth);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraManager)
	{
		return;
	}

	if (!HealthBarWidgetComponent)
	{
		return;
	}

	FRotator NewComponentRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarWidgetComponent->GetComponentLocation(), CameraManager->GetCameraLocation());
	HealthBarWidgetComponent->SetWorldRotation(NewComponentRotation);
}

void AEnemy::TakeDamage_Implementation(float DamageTaken)
{
	CurrentHealth = CurrentHealth - DamageTaken;

	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0;
		bIsDead = true;
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetBarPercent(CurrentHealth/MaxHealth);
	}
}

void AEnemy::Death_Implementation()
{
	SetDestination(GetActorLocation());

	OnEnemyDeath.Broadcast(ResourcesOnKill);

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
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

