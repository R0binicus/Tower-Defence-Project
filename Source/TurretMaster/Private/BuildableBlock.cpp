#include "BuildableBlock.h"

ABuildableBlock::ABuildableBlock()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABuildableBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

