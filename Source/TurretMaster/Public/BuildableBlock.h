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

	UPROPERTY(BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<USceneComponent> TurretHardpoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UStaticMeshComponent> BlockMesh;

	UPROPERTY(BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UStaticMeshComponent> BuildingPreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable Block")
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
	TScriptInterface<IBuildable> CreateBuildableActor(const TSubclassOf<AActor> BuildableClass);

	UFUNCTION()
	void OnCursorOverBegin(AActor* TouchedActor);

	UFUNCTION()
	void OnCursorOverEnd(AActor* TouchedActor);

	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Sets building preview mesh"))
	void SetBuildingPreview(UStaticMesh* PreviewMesh);

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Hides the building preview, and sets the mesh to be null"))
	void DisableBuildingPreview();
};
