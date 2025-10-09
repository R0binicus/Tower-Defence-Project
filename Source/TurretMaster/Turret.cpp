// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

    RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));

    RangeSphere->SetCollisionProfileName(TEXT("EnemyDetection"));
    RangeSphere->SetGenerateOverlapEvents(true);
    RangeSphere->SetSphereRadius(TurretRange);


    RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlapBegin);
    RangeSphere->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
    TurretLocation = GetActorLocation();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    CurrentClosestEnemy = GetClosestEnemy();

    if (CurrentClosestEnemy)
    {

    }
    
}

void ATurret::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    
    EnemyRefArray.Add(OtherActor);
}

void ATurret::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    EnemyRefArray.Remove(OtherActor);

}

AActor* ATurret::GetClosestEnemy()
{
    TObjectPtr<AActor> PotentialClosestEnemy = nullptr;
    float CurrentClosestDistance;

    for (size_t i = 0; i < EnemyRefArray.Num(); i++)
    {
        if (!EnemyRefArray[i])
        {
            continue;
        }

        if (!PotentialClosestEnemy)
        {
            CurrentClosestDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretLocation);
            PotentialClosestEnemy = EnemyRefArray[i];
        }
        else
        {
            float EnemyDistance = FVector::DistSquared(EnemyRefArray[i]->GetActorLocation(), TurretLocation);

            if (EnemyDistance <= CurrentClosestDistance)
            {
                continue;
            }

            CurrentClosestDistance = EnemyDistance;
            PotentialClosestEnemy = EnemyRefArray[i];
        }
    }

    return PotentialClosestEnemy;
}

