#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TowerDefencePlayerState.generated.h"

UENUM()
enum class EPlayerStateEnum
{
	Default,
	Building,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, EPlayerStateEnum, NewPlayerState);

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

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Gets the TowerDefencePlayerState's PlayerStateEnum"))
	EPlayerStateEnum GetPlayerState() const { return PlayerStateEnum; };

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Tries to set the player state to be the a new value"))
	bool TrySetPlayerState(const EPlayerStateEnum NewState);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TowerDefencePlayerState")
	EPlayerStateEnum PlayerStateEnum;

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerState",
		meta = (ToolTip = "Updates player state and sends the event"))
	void SetPlayerState(const EPlayerStateEnum NewState);
};
