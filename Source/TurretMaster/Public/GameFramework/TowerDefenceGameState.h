#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TowerDefenceGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameWin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLose);

/**
 * Game state for the tower defence game.
 * Triggers win + loss conditions
 */
UCLASS()
class TURRETMASTER_API ATowerDefenceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefenceGameState")
	FOnGameWin OnGameWin;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefenceGameState")
	FOnGameLose OnGameLose;
	
	UFUNCTION(BlueprintCallable, Category = "TowerDefenceGameState",
		meta = (ToolTip = "Triggers the win condition, sending events to everything subscribed"))
	void TriggerWin();

	UFUNCTION(BlueprintCallable, Category = "TowerDefenceGameState",
		meta = (ToolTip = "Triggers the lose condition, sending events to everything subscribed"))
	void TwiggerLose();
};
