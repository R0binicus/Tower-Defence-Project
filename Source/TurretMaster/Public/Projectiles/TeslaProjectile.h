#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TeslaProjectile.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

// TODO: Make this into a class which doesn't inherit from AProjectile, and make a 'TurretSpawnable' interface

/**
 * Projectile variant for the tesla turret
 */
UCLASS()
class TURRETMASTER_API ATeslaProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ATeslaProjectile();

	virtual void SetProjectileEnabled(bool bNewEnabled) override;

	virtual void SetupProjectile(AEnemy* Enemy, const FProjectileValues& InProjectileValues) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	int32 AllowedTeslaBounces = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	float BounceDamageReductionMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	TArray<TObjectPtr<UNiagaraComponent>> LightningBeamVFXArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	TEnumAsByte<EObjectTypeQuery> EnemyCollisionType = ObjectTypeQuery8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	class UNiagaraSystem* LightningVFXType;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Finds a set number of valid enemies, deals damage to them, and makes lightning VFX appear between them"))
	void BounceToTargets(const int32 NumberOfTargets);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Finds a valid enemy to bounce to, then bounces to it"))
	AEnemy* FindClosestEnemy(const FVector& CheckOriginPoint, const TArray<AEnemy*>& AlreadyTargetedEnemies) const;

	UFUNCTION(BlueprintCallable, Category = "TeslaProjectile",
		meta = (ToolTip = "Makes the specified number of niagara lightning VFX"))
	void MakeLightningVFX(const int NewVFXAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Creates a new lightning VFX and adds it to the array"))
	UNiagaraComponent* CreateLightningVFX();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Uses an existing niagara component to spawn lightning VFX "))
	void UseLightningVFX(const FVector& StartPosition, const FVector& EndPosition);

	int32 CurrentVFXIndex;
};
