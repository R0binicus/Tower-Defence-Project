#include "BuildableBlock.h"

ABuildableBlock::ABuildableBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Buildable Block Mesh"));
	BlockMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	RootComponent = BlockMesh;

	TurretHardpoint = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Hardpoint"));
	TurretHardpoint->SetupAttachment(RootComponent);

    BuildingPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Preview Mesh"));
    BuildingPreviewMesh->SetupAttachment(TurretHardpoint);
    BuildingPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    OnBeginCursorOver.AddDynamic(this, &ABuildableBlock::OnCursorOverBegin);
    OnEndCursorOver.AddDynamic(this, &ABuildableBlock::OnCursorOverEnd);
    OnClicked.AddDynamic(this, &ABuildableBlock::OnActorClicked);
}

void ABuildableBlock::BeginPlay()
{
	Super::BeginPlay();

    World = GetWorld();
	
    //CreatedBuildable = CreateBuildableActor(TestStartBuilding);
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

    //TODO: Disucss, can I check if it implements UBuildable, before I spawn it? 

    const TObjectPtr<AActor> BuildingActor = World->SpawnActor<AActor>(BuildableClass, TurretHardpoint->GetComponentLocation(), FRotator::ZeroRotator);
    if (!BuildingActor->Implements<UBuildable>())
    {
        return nullptr;
    }

    const TScriptInterface<IBuildable> Building = TScriptInterface<IBuildable>(BuildingActor);

    if (!Building)
    {
        return nullptr;
    }

    return Building;
}

void ABuildableBlock::OnCursorOverBegin(AActor* TouchedActor)
{
    // Don't create building preview if one already exists
    if (CreatedBuildable)
    {
        return;
    }

    if (!BuildingPreviewMesh)
    {
        return;
    }

    SetBuildingPreview(BlockMesh->GetStaticMesh());
}

void ABuildableBlock::OnCursorOverEnd(AActor* TouchedActor)
{
    // Don't create building preview if one already exists
    if (CreatedBuildable)
    {
        return;
    }

    if (!BuildingPreviewMesh)
    {
        return;
    }

    DisableBuildingPreview();
}

void ABuildableBlock::OnActorClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    // Don't create new building if one already exists
    if (CreatedBuildable)
    {
        return;
    }

    CreatedBuildable = CreateBuildableActor(TestStartBuilding);

    DisableBuildingPreview();
}

void ABuildableBlock::SetBuildingPreview(UStaticMesh* PreviewMesh)
{
    if (!PreviewMesh)
    {
        return;
    }

    if (!BuildingPreviewMesh)
    {
        return;
    }

    BuildingPreviewMesh->SetStaticMesh(PreviewMesh);
    BuildingPreviewMesh->SetMaterial(0, PreviewMaterial);
}

void ABuildableBlock::DisableBuildingPreview()
{
    BuildingPreviewMesh->SetStaticMesh(nullptr);
}

