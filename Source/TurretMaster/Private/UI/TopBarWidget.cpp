#include "UI/TopBarWidget.h"
#include "Subsystems/EnemySubsystem.h"

void UTopBarWidget::NativeConstruct()
{
    TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
    if (!EnemySubsystem)
    {
        return;
    }

    EnemySubsystem->OnEnemiesRemainingChanged.AddUniqueDynamic(this, &UTopBarWidget::UpdateEnemiesRemainingText);
}

void UTopBarWidget::NewWaveStarted(const int32 NewWavesRemaining, const int32 NewWaveEnemies)
{

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
        FString FormattedNum = FString::Printf(TEXT("%i/%i"), TotalWaveEnemies, NewEnemiesRemaining);
        EnemiesRemainingText->SetText(FText::FromString(FormattedNum));
    }
}
