#include "LivesLossArea.h"
#include "Enemies/Enemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/TowerDefencePlayerState.h"
#include "Kismet/GameplayStatics.h"

ALivesLossArea::ALivesLossArea()
{
    EnemyDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Detection Box"));
    EnemyDetectionBox->SetCollisionProfileName(TEXT("EnemyDetection"));
    EnemyDetectionBox->SetGenerateOverlapEvents(true);
    EnemyDetectionBox->SetBoxExtent(FVector(500, 500, 100), false);
    EnemyDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ALivesLossArea::OnOverlapBegin);
    RootComponent = EnemyDetectionBox;
}

void ALivesLossArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->ActorHasTag(EnemyTagName))
    {
        return;
    }

    TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    TObjectPtr<AEnemy> Enemy = Cast<AEnemy>(OtherActor);
    if (!Enemy || !PlayerState)
    {
        return;
    }

    PlayerState->ChangeCurrentLives(-Enemy->GetLivesReduction());
    Enemy->Death(false);
}
