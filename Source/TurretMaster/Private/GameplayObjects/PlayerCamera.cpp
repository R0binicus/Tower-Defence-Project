#include "GameplayObjects/PlayerCamera.h"
#include "Camera/CameraComponent.h"

APlayerCamera::APlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	RootComponent = PlayerCamera;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}
