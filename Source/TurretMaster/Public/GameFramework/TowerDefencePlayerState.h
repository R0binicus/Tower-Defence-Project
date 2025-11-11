#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "TowerDefencePlayerState.generated.h"

UENUM()
enum class EPlayerStateEnum
{
	Default,
	Building,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, EPlayerStateEnum, NewPlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLivesChanged, int32, NewPlayerLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoneyChanged, int32, NewPlayerMoney);

/**
 * Player state for the tower defence game
 */
UCLASS()
class TURRETMASTER_API ATowerDefencePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefencePlayerState")
	FOnPlayerStateChanged OnPlayerStateChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefencePlayerState")
	FOnPlayerLivesChanged OnPlayerLivesChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefencePlayerState")
	FOnPlayerMoneyChanged OnPlayerMoneyChanged;

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Gets the TowerDefencePlayerState's PlayerStateEnum"))
	EPlayerStateEnum GetPlayerState() const { return PlayerStateEnum; };

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Tries to set the player state to be the a new value"))
	bool TrySetPlayerState(const EPlayerStateEnum NewState);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Gets the number of player starting lives"))
	int32 GetPlayerLivesInitial() const { return PlayerLivesInitial; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Gets the number of current player lives"))
	int32 GetPlayerLivesCurrent() const { return PlayerLivesCurrent; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Gets the number of current money the player has"))
	int32 GetPlayerMoneyCurrent() const { return PlayerMoneyCurrent; };

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void SetPlayerLivesCurrent(const int32 NewLives);

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void ChangeCurrentMoney(const int32 MoneyChange);

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Changes the number of current lives the player has"))
	void ChangeCurrentLives(const int32 LiveChange);

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void SetPlayerMoneyCurrent(const int32 NewMoney);

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Checks if the player has enough resources for the cost"))
	bool HasEnoughResources(const int32 Cost) const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TowerDefencePlayerState")
	EPlayerStateEnum PlayerStateEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefencePlayerState")
	int32 PlayerLivesInitial = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefencePlayerState")
	int32 PlayerLivesCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefencePlayerState")
	int32 PlayerMoneyInitial = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefencePlayerState")
	int32 PlayerMoneyCurrent;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Updates player state and sends the event"))
	void SetPlayerState(const EPlayerStateEnum NewState);
};
