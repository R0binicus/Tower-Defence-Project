#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCamera.generated.h"

UCLASS()
class TURRETMASTER_API APlayerCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
