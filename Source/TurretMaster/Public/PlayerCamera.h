#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "PlayerCamera.generated.h"

UCLASS()
class TURRETMASTER_API APlayerCamera : public APawn
{
	GENERATED_BODY()
	
public:	
	APlayerCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Camera")
	TObjectPtr<UCameraComponent> PlayerCamera;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
