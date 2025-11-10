#include "UI/TopBarWidget.h"
#include "Subsystems/EnemySubsystem.h"

void UTopBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Set default text values
    NewWaveStarted(nullptr, 0);
    UpdateEnemiesRemainingText(0);

    TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
    if (EnemySubsystem)
    {
        TotalWaveNum = EnemySubsystem->GetTotalWaveNum();

        EnemySubsystem->OnWaveChanged.AddUniqueDynamic(this, &UTopBarWidget::NewWaveStarted);
        EnemySubsystem->OnEnemiesRemainingChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateEnemiesRemainingText);
    }

    TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    if (PlayerState)
    {
        UpdateLivesText(PlayerState->GetPlayerLivesCurrent());
        UpdateMoneyText(PlayerState->GetPlayerMoneyCurrent());

        PlayerState->OnPlayerLivesChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateLivesText);
        PlayerState->OnPlayerMoneyChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateMoneyText);
    }
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
    if (LivesText)
    {
        FString FormattedNum = FString::Printf(TEXT("%i"), NewLives);
        LivesText->SetText(FText::FromString(FormattedNum));
    }
}

void UTopBarWidget::UpdateMoneyText(const int32 NewMoney)
{
    if (MoneyText)
    {
        FString FormattedNum = FString::Printf(TEXT("%i"), NewMoney);
        MoneyText->SetText(FText::FromString(FormattedNum));
    }
}

void UTopBarWidget::UpdateEnemiesRemainingText(const int32 NewEnemiesRemaining)
{
    if (EnemiesRemainingText)
    {
        FString FormattedNum = FString::Printf(TEXT("%i/%i"), NewEnemiesRemaining, TotalWaveEnemies);
        EnemiesRemainingText->SetText(FText::FromString(FormattedNum));
    }
}
