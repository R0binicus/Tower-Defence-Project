#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLivesChanged, int32, NewPlayerLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoneyChanged, int32, NewPlayerMoney);

/**
 * Subsystem for handling player information, such as lives and money
 */
UCLASS()
class TURRETMASTER_API UPlayerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "PlayerSubsystem")
	FOnPlayerLivesChanged OnPlayerLivesChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "PlayerSubsystem")
	FOnPlayerMoneyChanged OnPlayerMoneyChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerSubsystem",
		meta = (ToolTip = "Gets the number of player starting lives"))
	int32 GetPlayerLivesInitial() const { return PlayerLivesInitial; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerSubsystem",
		meta = (ToolTip = "Gets the number of current player lives"))
	int32 GetPlayerLivesCurrent() const { return PlayerLivesCurrent; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerSubsystem",
		meta = (ToolTip = "Gets the number of current money the player has"))
	int32 GetPlayerMoneyCurrent() const { return PlayerMoneyCurrent; };

	UFUNCTION(BlueprintCallable, Category = "PlayerSubsystem",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void SetPlayerLivesCurrent(const int32 NewLives);

	UFUNCTION(BlueprintCallable, Category = "PlayerSubsystem",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void SetPlayerMoneyCurrent(const int32 NewMoney);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSubsystem")
	int32 PlayerLivesInitial = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSubsystem")
	int32 PlayerLivesCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSubsystem")
	int32 PlayerMoneyInitial = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSubsystem")
	int32 PlayerMoneyCurrent;
};
