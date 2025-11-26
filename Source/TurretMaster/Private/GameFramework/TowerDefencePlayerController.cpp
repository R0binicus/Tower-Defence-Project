#include "GameFramework/TowerDefencePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ATowerDefencePlayerController::BeginPlay()
{
	const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputSubsystem)
	{
		InputSubsystem->AddMappingContext(InputMap, 0);
	}
}

void ATowerDefencePlayerController::SetupInputComponent()
{
	APlayerController::SetupInputComponent();

	const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	if (!PauseAction || !QueueAction || !CancelAction || !SelectAction || !ThumbstickMoveAction)
	{
		return;
	}

	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::PauseInputAction);
	EnhancedInputComponent->BindAction(QueueAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::QueueInputAction);
	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::CancelInputAction);
	EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ATowerDefencePlayerController::DeselectInputAction);
	EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ATowerDefencePlayerController::SelectInputAction);
	EnhancedInputComponent->BindAction(ThumbstickMoveAction, ETriggerEvent::Triggered, this, &ATowerDefencePlayerController::SelectThumbstickMoveAction);
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

void ATowerDefencePlayerController::SelectThumbstickMoveAction(const FInputActionValue& Value)
{
	FVector2D ThumbstickVector = Value.Get<FVector2D>();

	float NewMouseXPos;
	float NewMouseYPos;
	GetMousePosition(NewMouseXPos, NewMouseYPos);
	NewMouseXPos = NewMouseXPos + (ThumbstickVector.X * ThumbstickMoveMultiplier);
	NewMouseYPos = NewMouseYPos - (ThumbstickVector.Y * ThumbstickMoveMultiplier);

	SetMouseLocation(NewMouseXPos, NewMouseYPos);
}
