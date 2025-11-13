#include "GameFramework/TowerDefencePlayerController.h"

void ATowerDefencePlayerController::BeginPlay()
{

}

void ATowerDefencePlayerController::SetupInputComponent()
{
	APlayerController::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	if (PauseAction)
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::PauseInputAction);
	}

	if (QueueAction)
	{
		EnhancedInputComponent->BindAction(QueueAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::QueueInputAction);
	}

	if (CancelAction)
	{
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::CancelInputAction);
	}
}

void ATowerDefencePlayerController::PauseInputAction()
{
	OnPauseInput.Broadcast();
}

void ATowerDefencePlayerController::QueueInputAction()
{
	OnQueueInput.Broadcast();
}

void ATowerDefencePlayerController::CancelInputAction()
{
	OnCancelInput.Broadcast();
}
