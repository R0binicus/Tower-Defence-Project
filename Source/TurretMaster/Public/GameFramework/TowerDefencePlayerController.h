#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TowerDefencePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseInput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQueueInput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelInput);

/**
 * Player Controller for the tower defence game.
 */
UCLASS()
class TURRETMASTER_API ATowerDefencePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefencePlayerController")
	FOnPauseInput OnPauseInput;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefencePlayerController")
	FOnQueueInput OnQueueInput;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefencePlayerController")
	FOnCancelInput OnCancelInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerDefencePlayerController")
	TObjectPtr<UInputMappingContext> InputMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerDefencePlayerController")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerDefencePlayerController")
	TObjectPtr<UInputAction> QueueAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerDefencePlayerController")
	TObjectPtr<UInputAction> CancelAction;

	void BeginPlay() override;

	void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerController",
		meta = (ToolTip = "Broadcasts an event saying that the pause button was pressed"))
	void PauseInputAction();

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerController",
		meta = (ToolTip = "Broadcasts an event saying that the queue button was pressed"))
	void QueueInputAction();

	UFUNCTION(BlueprintCallable, Category = "TowerDefencePlayerController",
		meta = (ToolTip = "Broadcasts an event saying that the cancel button was pressed"))
	void CancelInputAction();
};
