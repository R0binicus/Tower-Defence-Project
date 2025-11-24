#include "Enemy.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "UI/EnemyHealthbarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController || !HealthBarWidgetComponent || !HealthBarWidget)
	{
		CameraManager = PlayerController->PlayerCameraManager;
	}

	HealthBarWidget = Cast<UEnemyHealthbarWidget>(HealthBarWidgetComponent->GetWidget());

	// Scale widget to actor size
	const FVector EnemyScale = GetActorScale();
	const float ScaleAvg = (EnemyScale.X + EnemyScale.Y + EnemyScale.Z) / 3;
	const FVector2D NewWidgetScale = FVector2D(HealthbarScaleMin.X + (ScaleAvg * HealthbarScaleMultiplier.X), 
		HealthbarScaleMin.Y + (ScaleAvg * HealthbarScaleMultiplier.Y));
	HealthBarWidget->SetRenderScale(NewWidgetScale);
}

void AEnemy::Tick(const float DeltaTime)
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

	const FRotator NewComponentRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarWidgetComponent->GetComponentLocation(), CameraManager->GetCameraLocation());
	HealthBarWidgetComponent->SetWorldRotation(NewComponentRotation);
}

void AEnemy::TakeDamage_Implementation(const float DamageTaken)
{
	CurrentHealth = CurrentHealth - DamageTaken;

	if (CurrentHealth <= 0.f)
	{
		Death(true);
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetBarPercent(CurrentHealth/MaxHealth);
	}
}

void AEnemy::Death_Implementation(const bool bByTurret)
{
	SetDestination(GetActorLocation());
	CurrentHealth = 0;
	bIsDead = true;

	if (bByTurret)
	{
		OnEnemyDeath.Broadcast(ResourcesOnKill);
	}
	else
	{
		OnEnemyDeath.Broadcast(0);
	}

	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::SetDestination(const FVector NewDestination) const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(NewDestination, 5.f, false, true, true, false);
	}
	
}
