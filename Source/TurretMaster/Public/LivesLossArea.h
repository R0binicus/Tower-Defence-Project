#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LivesLossArea.generated.h"

/**
 * Area in the level which reduces player lives whenan enemy enters it
 */
UCLASS()
class TURRETMASTER_API ALivesLossArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ALivesLossArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
