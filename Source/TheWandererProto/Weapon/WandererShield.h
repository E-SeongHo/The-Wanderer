//  

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WandererEquipment.h"
#include "WandererShield.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class THEWANDERERPROTO_API AWandererShield : public AWandererEquipment
{
	GENERATED_BODY()

public:
	AWandererShield();

protected:
	UPROPERTY(EditAnywhere, Category = Shield)
	TObjectPtr<UStaticMeshComponent> ShieldMesh;
};