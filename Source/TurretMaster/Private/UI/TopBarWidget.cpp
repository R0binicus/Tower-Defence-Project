#include "UI/TopBarWidget.h"
#include "Subsystems/EnemySubsystem.h"

void UTopBarWidget::NativeConstruct()
{
    TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
    if (!EnemySubsystem)
    {
        return;
    }

    TotalWaveNum = EnemySubsystem->GetTotalWaveNum();

    EnemySubsystem->OnWaveChanged.AddUniqueDynamic(this, &UTopBarWidget::NewWaveStarted);
    EnemySubsystem->OnEnemiesRemainingChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateEnemiesRemainingText);
}

void UTopBarWidget::NewWaveStarted(UWaveDataObject* NewWaveData, const int32 NewWaveNum)
{
    if (WavesRemainingText)
    {
        FString FormattedNum = FString::Printf(TEXT("%i/%i"), NewWaveNum, TotalWaveNum);
        WavesRemainingText->SetText(FText::FromString(FormattedNum));
    }

    if (!NewWaveData)
    {
        return;
    }

    FEnemyWaveData& WaveData = NewWaveData->WaveData;

    int32 TotalNumber = 0;
    for (const TPair<TSubclassOf<AEnemy>, int32>& Pair : WaveData.EnemyAmounts)
    {
        TotalNumber = TotalNumber + Pair.Value;
    }

    TotalWaveEnemies = TotalNumber;
    UpdateEnemiesRemainingText(TotalWaveEnemies);
}

void UTopBarWidget::UpdateLivesText(const int32 NewLives)
{

}

void UTopBarWidget::UpdateMoneyText(const int32 NewMoney)
{

}

void UTopBarWidget::UpdateEnemiesRemainingText(const int32 NewEnemiesRemaining)
{
    if (EnemiesRemainingText)
    {
        FString FormattedNum = FString::Printf(TEXT("%i/%i"), NewEnemiesRemaining, TotalWaveEnemies);
        EnemiesRemainingText->SetText(FText::FromString(FormattedNum));
    }
}
