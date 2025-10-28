#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildableBlock.generated.h"

class UStaticMeshComponent;

UCLASS()
class TURRETMASTER_API ABuildableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildableBlock();

protected:
	// Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<USceneComponent> TurretHardpoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Buildable Block")
	TObjectPtr<UStaticMeshComponent> BlockMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UObject> CreatedBuildable = nullptr;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
