#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/TowerDefencePlayerState.h"
#include "BuildableBlock.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UBuildable;
class UBuildingDataAsset;

UCLASS()
class TURRETMASTER_API ABuildableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildableBlock();

protected:
	// Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<USceneComponent> SceneComponentRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<USceneComponent> TurretHardpoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UStaticMeshComponent> BlockMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buildable Block")
	TObjectPtr<USphereComponent> RangePreviewComponent;

	//TODO: Discuss, how does this get set to null???

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	//TObjectPtr<USkeletalMeshComponent> BuildingPreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<USkeletalMeshComponent> BuildingPreviewMeshNew;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TScriptInterface<IBuildable> CreatedBuildable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	TObjectPtr<UBuildingDataAsset> BuildingDataAsset;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Buildable Block")
	EPlayerStateEnum PlayerState = EPlayerStateEnum::Default;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Creates and initialises a buildable actor"))
	TScriptInterface<IBuildable> CreateBuildableActor(const TSubclassOf<AActor> BuildableClass) const;

	UFUNCTION()
	void OnCursorOverBegin(AActor* TouchedActor);

	UFUNCTION()
	void OnCursorOverEnd(AActor* TouchedActor);

	UFUNCTION()
	void OnActorClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Sets building preview mesh"))
	void SetBuildingAsset(UBuildingDataAsset* NewBuilding);

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Sets building preview mesh"))
	void SetBuildingPreview(USkeletalMesh* PreviewMesh) const;

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Hides the building preview, and sets the mesh to be null"))
	void DisableBuildingPreview() const;

	UFUNCTION(BlueprintCallable, Category = "Buildable Block",
		meta = (ToolTip = "Sets the internal player state variable"))
	void SetPlayerState(const EPlayerStateEnum NewState, const EPlayerStateEnum OldState);
};
