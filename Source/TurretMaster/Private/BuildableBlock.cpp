#include "BuildableBlock.h"
#include "Buildable.h"
#include "DataAssets/BuildingDataAsset.h"
#include "Subsystems/BuildingSubsystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"

ABuildableBlock::ABuildableBlock()
{
	PrimaryActorTick.bCanEverTick = true;

    SceneComponentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component Root"));
    RootComponent = SceneComponentRoot;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Buildable Block Mesh"));
	BlockMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
    BlockMesh->SetupAttachment(RootComponent);
	
	TurretHardpoint = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Hardpoint"));
	TurretHardpoint->SetupAttachment(RootComponent);

    RangePreviewComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RangePreviewComponent"));
    RangePreviewComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RangePreviewComponent->SetGenerateOverlapEvents(false);
    RangePreviewComponent->SetupAttachment(TurretHardpoint);

    //BuildingPreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Building Preview Mesh"));
    //BuildingPreviewMesh->SetupAttachment(TurretHardpoint);
    //BuildingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    BuildingPreviewMeshNew = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Building Preview Mesh New"));
    BuildingPreviewMeshNew->SetupAttachment(TurretHardpoint);
    BuildingPreviewMeshNew->SetRelativeRotation(FRotator(0, 270, 0));
    BuildingPreviewMeshNew->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    OnBeginCursorOver.AddDynamic(this, &ABuildableBlock::OnCursorOverBegin);
    OnEndCursorOver.AddDynamic(this, &ABuildableBlock::OnCursorOverEnd);
}

void ABuildableBlock::BeginPlay()
{
	Super::BeginPlay();

    World = GetWorld();

    const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    const TObjectPtr<ATowerDefencePlayerState> PlayerStateClass = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    const TObjectPtr<ATowerDefencePlayerController> PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (!BuildingSubsystem || !PlayerStateClass || !PlayerController)
    {
        return;
    }
    
    PlayerStateClass->OnPlayerStateChanged.AddUniqueDynamic(this, &ABuildableBlock::SetPlayerState);
    BuildingSubsystem->OnBuildingTypeSelected.AddUniqueDynamic(this, &ABuildableBlock::SetBuildingAsset);
    PlayerController->OnSelectInput.AddUniqueDynamic(this, &ABuildableBlock::OnActorClicked);
}

TScriptInterface<IBuildable> ABuildableBlock::CreateBuildableActor(const TSubclassOf<AActor> BuildableClass) const
{
    if (!World || !BuildableClass)
    {
        return nullptr;
    }

    if (!BuildableClass->ImplementsInterface(UBuildable::StaticClass()))
    {
        return nullptr;
    }

    const TObjectPtr<AActor> BuildingActor = World->SpawnActor<AActor>(BuildableClass, TurretHardpoint->GetComponentLocation(), FRotator::ZeroRotator);
    const TScriptInterface<IBuildable> Building = TScriptInterface<IBuildable>(BuildingActor);
    const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (!BuildingActor || !Building || !BuildingSubsystem)
    {
        return nullptr;
    }

    IBuildable::Execute_SetProtectPoint(BuildingActor, BuildingSubsystem->GetProtectPoint());

    return Building;
}

void ABuildableBlock::OnCursorOverBegin(AActor* TouchedActor)
{
    bMouseHoveringOver = true;

    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // Don't create building preview if one already exists
    if (IsValid(CreatedBuildable.GetObject()))
    {
        return;
    }

    if (!BuildingDataAsset || !RangePreviewComponent)
    {
        return;
    }

    const TObjectPtr<USkeletalMesh> SkeletalMesh = BuildingDataAsset->SkeletalMesh;
    if (!SkeletalMesh)
    {
        return;
    }

    SetBuildingPreview(SkeletalMesh);

    RangePreviewComponent->SetSphereRadius(BuildingDataAsset->RangePreview);
    RangePreviewComponent->SetHiddenInGame(false);
}

void ABuildableBlock::OnCursorOverEnd(AActor* TouchedActor)
{
    bMouseHoveringOver = false;

    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // Don't create building preview if one already exists
    if (IsValid(CreatedBuildable.GetObject()))
    {
        return;
    }

    if (!BuildingPreviewMeshNew || !RangePreviewComponent)
    {
        return;
    }

    DisableBuildingPreview();
    RangePreviewComponent->SetHiddenInGame(true);
}

void ABuildableBlock::OnActorClicked()
{
    if (!bMouseHoveringOver)
    {
        return;
    }

    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // Don't create new building if one already exists
    if (IsValid(CreatedBuildable.GetObject()))
    {
        return;
    }

    if (!BuildingDataAsset)
    {
        return;
    }

    const TSubclassOf<AActor> BuildableClass = BuildingDataAsset->Class;
    if (!BuildableClass)
    {
        return;
    }

    CreatedBuildable = CreateBuildableActor(BuildableClass);

    const TObjectPtr<ATowerDefencePlayerState> PlayerStateClass = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    
    if (!PlayerStateClass || !BuildingSubsystem)
    {
        return;
    }

    PlayerStateClass->ChangeCurrentMoney(-BuildingDataAsset->Cost);
    BuildingSubsystem->BuildingPlaced();
    DisableBuildingPreview();
}

void ABuildableBlock::SetBuildingAsset(UBuildingDataAsset* NewBuilding)
{
    BuildingDataAsset = NewBuilding;
}

void ABuildableBlock::SetBuildingPreview(USkeletalMesh* PreviewMesh) const
{
    if (!PreviewMesh || !BuildingPreviewMeshNew)
    {
        return;
    }

    BuildingPreviewMeshNew->SetSkeletalMesh(PreviewMesh);
    for (size_t i = 0; i < BuildingPreviewMeshNew->GetNumMaterials(); i++)
    {
        BuildingPreviewMeshNew->SetMaterial(i, PreviewMaterial);
    }
}

void ABuildableBlock::DisableBuildingPreview() const
{
    if (!BuildingPreviewMeshNew || !RangePreviewComponent)
    {
        return;
    }

    BuildingPreviewMeshNew->SetSkeletalMesh(nullptr);
    RangePreviewComponent->SetHiddenInGame(true);
}

void ABuildableBlock::SetPlayerState(const EPlayerStateEnum NewState, const EPlayerStateEnum OldState)
{
    PlayerState = NewState;

    if (PlayerState != EPlayerStateEnum::Building)
    {
        DisableBuildingPreview();
    }
}