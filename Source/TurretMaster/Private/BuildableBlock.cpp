#include "BuildableBlock.h"
#include "Components/SphereComponent.h"

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
    OnClicked.AddDynamic(this, &ABuildableBlock::OnActorClicked);
}

void ABuildableBlock::BeginPlay()
{
	Super::BeginPlay();

    World = GetWorld();

    TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (BuildingSubsystem)
    {
        BuildingSubsystem->OnBuildingTypeSelected.AddUniqueDynamic(this, &ABuildableBlock::SetBuildingAsset);
    }

    TObjectPtr<ATowerDefencePlayerState> PlayerStateClass = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    if (PlayerStateClass)
    {
        PlayerStateClass->OnPlayerStateChanged.AddUniqueDynamic(this, &ABuildableBlock::SetPlayerState);
    }
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

    TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (BuildingSubsystem)
    {
        IBuildable::Execute_SetProtectPoint(BuildingActor, BuildingSubsystem->GetProtectPoint());
    }

    return Building;
}

void ABuildableBlock::OnCursorOverBegin(AActor* TouchedActor)
{
    if (PlayerState != EPlayerStateEnum::Building)
    {
        return;
    }

    // TODO: Discuss, IsValid vs !Object and the minimap icon object pooling

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

void ABuildableBlock::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
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

    TObjectPtr<ATowerDefencePlayerState> PlayerStateClass = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    if (!PlayerStateClass)
    {
        return;
    }

    PlayerStateClass->ChangeCurrentMoney(-BuildingDataAsset->Cost);

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