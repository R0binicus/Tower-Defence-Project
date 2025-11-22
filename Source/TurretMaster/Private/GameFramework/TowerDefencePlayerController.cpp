#include "GameFramework/TowerDefencePlayerController.h"

void ATowerDefencePlayerController::BeginPlay()
{
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSubsystem)
	{
		InputSubsystem->AddMappingContext(InputMap, 0);
	}
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

	if (SelectAction)
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::DeselectInputAction);
	}

	if (SelectAction)
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ATowerDefencePlayerController::SelectInputAction);
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

void ATowerDefencePlayerController::DeselectInputAction()
{
	OnDeselectInput.Broadcast();
}

void ATowerDefencePlayerController::SelectInputAction()
{
	OnSelectInput.Broadcast();
}
