//  

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WandererWeapon.h"
#include "WandererSword.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class THEWANDERERPROTO_API AWandererSword : public AWandererWeapon
{
	GENERATED_BODY()

public:
	AWandererSword();
	virtual bool Trace(FHitResult& OutHit) override;
	
private:
	UPROPERTY(EditAnywhere, Category = Sheath)
	TObjectPtr<UStaticMeshComponent> SheathMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UStaticMeshComponent> SwordMesh;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UArrowComponent> BladeStart;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UArrowComponent> BladeEnd;
};
