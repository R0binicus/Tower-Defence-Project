#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildableBlock.generated.h"

UCLASS()
class TURRETMASTER_API ABuildableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildableBlock();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
