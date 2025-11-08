#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
//#include "EnemyWaveData.h"
#include "TopBarWidget.generated.h"

/**
 * Widget which is at the top of the screen. Displays information such as money and player lives
 */
UCLASS()
class TURRETMASTER_API UTopBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TopBarWidget")
	int32 TotalWaveNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TopBarWidget")
	int32 TotalWaveEnemies;

	//TODO: Discuss, why doesn't this recognise the struct? is it because of the folder structure?

	//UPROPERTY(BlueprintReadWrite, Category = "TopBarWidget")
	//TObjectPtr<FEnemyWaveData> CurrentWaveData;

	// Bound Widgets

	UPROPERTY(BlueprintReadWrite, Category = "TopBarWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> LivesText;

	UPROPERTY(BlueprintReadWrite, Category = "TopBarWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> MoneyText;

	UPROPERTY(BlueprintReadWrite, Category = "TopBarWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> WavesRemainingText;

	UPROPERTY(BlueprintReadWrite, Category = "TopBarWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemiesRemainingText;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates values due to a new wave starting. Called when OnWaveChanged event is triggered"))
	void NewWaveStarted(const int32 NewWavesRemaining, const int32 NewWaveEnemies);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates the LivesText. Called when OnLivesChanged event is triggered"))
	void UpdateLivesText(const int32 NewLives);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates the MoneyText. Called when OnMoneyChanged event is triggered"))
	void UpdateMoneyText(const int32 NewMoney);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates the EnemiesRemainingText. Called when OnEnemiesRemainingChanged event is triggered"))
	void UpdateEnemiesRemainingText(const int32 NewEnemiesRemaining);
};
