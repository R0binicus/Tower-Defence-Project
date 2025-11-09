#include "BuildableBlock.h"

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

    //BuildingPreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Building Preview Mesh"));
    //BuildingPreviewMesh->SetupAttachment(TurretHardpoint);
    //BuildingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    BuildingPreviewMeshNew = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Building Preview Mesh New"));
    BuildingPreviewMeshNew->SetupAttachment(TurretHardpoint);
    BuildingPreviewMeshNew->SetRelativeRotation(FRotator(0, 270, 0));
    BuildingPreviewMeshNew->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    OnBeginCursorOver.AddDynamic(this, &ABuildableBlock::OnCursorOverBegin);
    OnEndCursorOver.AddDynamic(this, &ABuildableBlock::OnCursorOverEnd);
    OnClicked.AddDynamic(this, &ABuildableBlock::OnActorClicked);
}

void ABuildableBlock::BeginPlay()
{
	Super::BeginPlay();

    World = GetWorld();

    TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (!BuildingSubsystem)
    {
        return;
    }

    BuildingSubsystem->OnBuildingTypeSelected.AddUniqueDynamic(this, &ABuildableBlock::SetBuildingAsset);

    // TODO: Discuss, will it break if I try to cast a nullptr?

    TObjectPtr<APlayerState> GenericPlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0);
    TObjectPtr<ATowerDefencePlayerState> CustomPlayerState = Cast<ATowerDefencePlayerState>(GenericPlayerState);
    if (!CustomPlayerState)
    {
        return;
    }

    CustomPlayerState->OnPlayerStateChanged.AddUniqueDynamic(this, &ABuildableBlock::SetPlayerState);
}

void ABuildableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TScriptInterface<IBuildable> ABuildableBlock::CreateBuildableActor(const TSubclassOf<AActor> BuildableClass)
{
    if (!BuildableClass)
    {
        return nullptr;
    }

    if (!World)
    {
        return nullptr;
    }

    if (!BuildableClass->ImplementsInterface(UBuildable::StaticClass()))
    {
        return nullptr;
    }

    const TObjectPtr<AActor> BuildingActor = World->SpawnActor<AActor>(BuildableClass, TurretHardpoint->GetComponentLocation(), FRotator::ZeroRotator);

    const TScriptInterface<IBuildable> Building = TScriptInterface<IBuildable>(BuildingActor);

    if (!Building)
    {
        return nullptr;
    }

    return Building;
}

void ABuildableBlock::OnCursorOverBegin(AActor* TouchedActor)
{
    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // Don't create building preview if one already exists
    if (CreatedBuildable)
    {
        return;
    }

    if (!BuildingDataAsset)
    {
        return;
    }

    const TObjectPtr<USkeletalMesh> SkeletalMesh = BuildingDataAsset->SkeletalMesh;

    if (!SkeletalMesh)
    {
        return;
    }

    SetBuildingPreview(SkeletalMesh);
}

void ABuildableBlock::OnCursorOverEnd(AActor* TouchedActor)
{
    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // Don't create building preview if one already exists
    if (CreatedBuildable)
    {
        return;
    }

    if (!BuildingPreviewMeshNew)
    {
        return;
    }

    DisableBuildingPreview();
}

void ABuildableBlock::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // Don't create new building if one already exists
    if (CreatedBuildable)
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

    DisableBuildingPreview();

    TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (!BuildingSubsystem)
    {
        return;
    }

    BuildingSubsystem->BuildingPlaced();
}

void ABuildableBlock::SetBuildingAsset(UBuildingDataAsset* NewBuilding)
{
    BuildingDataAsset = NewBuilding;
}

void ABuildableBlock::SetBuildingPreview(USkeletalMesh* PreviewMesh)
{
    if (!PreviewMesh)
    {
        return;
    }

    if (!BuildingPreviewMeshNew)
    {
        return;
    }

    BuildingPreviewMeshNew->SetSkeletalMesh(PreviewMesh);
    for (size_t i = 0; i < BuildingPreviewMeshNew->GetNumMaterials(); i++)
    {
        BuildingPreviewMeshNew->SetMaterial(i, PreviewMaterial);
    }
}

void ABuildableBlock::DisableBuildingPreview()
{
    if (!BuildingPreviewMeshNew)
    {
        return;
    }

    BuildingPreviewMeshNew->SetSkeletalMesh(nullptr);
}

void ABuildableBlock::SetPlayerState(const EPlayerStateEnum NewState)
{
    PlayerState = NewState;

    if (PlayerState != EPlayerStateEnum::Building)
    {
        DisableBuildingPreview();
    }
}