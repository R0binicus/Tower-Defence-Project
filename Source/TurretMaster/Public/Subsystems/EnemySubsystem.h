#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyWaveData.h"
#include "WaveDataObject.h"
#include "EnemySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveChanged, UWaveDataObject*, NewWaveData, int32, NewWaveNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemiesRemainingChanged, int32, NewEnemiesRemaining);

/**
 * Subsystem for handling enemy and wave information, such as enemies remaining in the level
 */
UCLASS()
class TURRETMASTER_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EnemySubsystem")
	FOnWaveChanged OnWaveChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EnemySubsystem")
	FOnEnemiesRemainingChanged OnEnemiesRemainingChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the the data for the current enemy wave"))
	UWaveDataObject* GetCurrentWaveData() const { return CurrentWaveData; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the number of Enemies Remaining"))
	int32 GetEnemiesRemaining() const { return EnemiesRemaining; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the current wave's number"))
	int32 GetCurrentWaveNum() const { return CurrentWaveNum; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the total number of enemy waves"))
	int32 GetTotalWaveNum() const { return TotalWaveNum; };

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void SetCurrentWaveData(UWaveDataObject* NewWaveData, const int32 NewWaveNum);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Changes the number of enemies remaining in the level"))
	void SetEnemiesRemaining(const int32 NewEnemiesRemaining);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Sets the total number of waves for the UI"))
	void SetTotalWaveNum(const int32 NewTotalWaveNum);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	TObjectPtr<UWaveDataObject> CurrentWaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	int32 EnemiesRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	int32 CurrentWaveNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	int32 TotalWaveNum;
};
