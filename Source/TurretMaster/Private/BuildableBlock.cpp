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
	
}

void ABuildableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

