#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWaveData.h"
#include "TopBarWidget.generated.h"

class UWaveDataObject;
class ULevelDataAsset;
class UTextBlock;

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

	UPROPERTY(BlueprintReadWrite, Category = "TopBarWidget")
	FEnemyWaveData CurrentWaveData;

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

	UFUNCTION()
	void OnLevelDataLoaded(ULevelDataAsset* LevelData);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates values due to a new wave starting. Called when OnWaveChanged event is triggered"))
	void NewWaveStarted(UWaveDataObject* NewWaveData, const int32 NewWaveNum);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates the LivesText. Called when OnLivesChanged event is triggered"))
	void UpdateLivesText(const int32 NewLives, const int32 OldLives);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates the MoneyText. Called when OnMoneyChanged event is triggered"))
	void UpdateMoneyText(const int32 NewMoney, const int32 OldMoney);

	UFUNCTION(BlueprintCallable, Category = "TopBarWidget",
		meta = (ToolTip = "Updates the EnemiesRemainingText. Called when OnEnemiesRemainingChanged event is triggered"))
	void UpdateEnemiesRemainingText(const int32 NewEnemiesRemaining);
};
