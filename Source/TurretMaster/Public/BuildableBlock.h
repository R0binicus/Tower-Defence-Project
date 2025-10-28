#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Buildable.h"
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
	TScriptInterface<IBuildable> CreatedBuildable = nullptr;

	//TODO: Disucss, is there a better way of doing this?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block", meta = (MustImplement = "Buildable"))
	TSubclassOf<AActor> TestStartBuilding;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Creates and initialises a buildable actor"))
	TScriptInterface<IBuildable> CreateBuildableActor(TSubclassOf<AActor> BuildableClass);

	UFUNCTION()
	void OnCursorOverBegin(AActor* TouchedActor);

	UFUNCTION()
	void OnCursorOverEnd(AActor* TouchedActor);

	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);
};
