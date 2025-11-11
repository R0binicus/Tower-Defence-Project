#include "LivesLossArea.h"

ALivesLossArea::ALivesLossArea()
{
    EnemyDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Detection Box"));
    EnemyDetectionBox->SetCollisionProfileName(TEXT("EnemyDetection"));
    EnemyDetectionBox->SetGenerateOverlapEvents(true);
    EnemyDetectionBox->SetBoxExtent(FVector(1000, 1000, 500), false);
    EnemyDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ALivesLossArea::OnOverlapBegin);
    RootComponent = EnemyDetectionBox;
}

void ALivesLossArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALivesLossArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->ActorHasTag(EnemyTagName))
    {
        return;
    }

    TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    if (PlayerState)
    {
        PlayerState->ChangeCurrentLives(-1);
    }
}
