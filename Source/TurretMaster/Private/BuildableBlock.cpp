#include "BuildableBlock.h"

ABuildableBlock::ABuildableBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Buildable Block Mesh"));
	BlockMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	RootComponent = BlockMesh;

	TurretHardpoint = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Hardpoint"));
	TurretHardpoint->SetupAttachment(RootComponent);
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

TScriptInterface<IBuildable> ABuildableBlock::CreateBuildableActor(TSubclassOf<AActor> BuildableClass)
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

