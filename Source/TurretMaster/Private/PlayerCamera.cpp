#include "PlayerCamera.h"

APlayerCamera::APlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

