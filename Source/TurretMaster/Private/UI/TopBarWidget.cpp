#include "UI/TopBarWidget.h"
#include "Subsystems/EnemySubsystem.h"
#include "Components/TextBlock.h"
#include "WaveDataObject.h"
#include "GameFramework/TowerDefenceGameInstance.h"
#include "GameFramework/TowerDefencePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PrimaryAssets/LevelDataAsset.h"

void UTopBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    const TObjectPtr<UWorld> World = GetWorld();
    if (!World)
    {
        return;
    }

    UTowerDefenceGameInstance* GameInstance = Cast<UTowerDefenceGameInstance>(World->GetGameInstance());
    const TObjectPtr<UEnemySubsystem> EnemySubsystem = World->GetSubsystem<UEnemySubsystem>();
    const TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(World, 0));

    if (!GameInstance || !EnemySubsystem || !PlayerState)
    {
        return;
    }
        
    GameInstance->OnLevelDataLoaded.AddUniqueDynamic(this, &UTopBarWidget::OnLevelDataLoaded);

    EnemySubsystem->OnWaveChanged.AddUniqueDynamic(this, &UTopBarWidget::NewWaveStarted);
    EnemySubsystem->OnEnemiesRemainingChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateEnemiesRemainingText);

    UpdateLivesText(PlayerState->GetPlayerLivesCurrent(), 0);
    UpdateMoneyText(PlayerState->GetPlayerMoneyCurrent(), 0);

    PlayerState->OnPlayerLivesChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateLivesText);
    PlayerState->OnPlayerMoneyChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateMoneyText);

    // Set default text value
    NewWaveStarted(nullptr, 0);
    UpdateEnemiesRemainingText(0);
}

void UTopBarWidget::OnLevelDataLoaded(ULevelDataAsset* LevelData)
{
    TotalWaveNum = LevelData->LevelWaveData.Num();
}

void UTopBarWidget::NewWaveStarted(UWaveDataObject* NewWaveData, const int32 NewWaveNum)
{
    if (!WavesRemainingText || !NewWaveData)
    {
        return;
    }

    const FString FormattedNum = FString::Printf(TEXT("%i/%i"), NewWaveNum, TotalWaveNum);
    WavesRemainingText->SetText(FText::FromString(FormattedNum));

    FEnemyWaveData& WaveData = NewWaveData->WaveData;

    int32 TotalNumber = 0;
    for (const TPair<TSubclassOf<AEnemy>, int32>& Pair : WaveData.EnemyAmounts)
    {
        TotalNumber = TotalNumber + Pair.Value;
    }

    TotalWaveEnemies = TotalNumber;
    UpdateEnemiesRemainingText(TotalWaveEnemies);
}

void UTopBarWidget::UpdateLivesText(const int32 NewLives, const int32 OldLives)
{
    if (LivesText)
    {
        const FString FormattedNum = FString::Printf(TEXT("%i"), NewLives);
        LivesText->SetText(FText::FromString(FormattedNum));
    }
}

void UTopBarWidget::UpdateMoneyText(const int32 NewMoney, const int32 OldMoney)
{
    if (MoneyText)
    {
        const FString FormattedNum = FString::Printf(TEXT("$%i"), NewMoney);
        MoneyText->SetText(FText::FromString(FormattedNum));
    }
}

void UTopBarWidget::UpdateEnemiesRemainingText(const int32 NewEnemiesRemaining)
{
    if (EnemiesRemainingText)
    {
        const FString FormattedNum = FString::Printf(TEXT("%i/%i"), NewEnemiesRemaining, TotalWaveEnemies);
        EnemiesRemainingText->SetText(FText::FromString(FormattedNum));
    }
}
