//  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WandererWeapon.generated.h"

class UArrowComponent;

UCLASS()
class THEWANDERERPROTO_API AWandererWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWandererWeapon();
	UStaticMeshComponent* GetWeaponMesh() const {return WeaponMesh.Get();}

	// TODO : Think about another options...
	// 1) define class RangedWeapon
	// 2) define interface Traceable
	// 3) Grant Ability to weapon  
	virtual bool Trace(FHitResult& OutHit) { check(false); return false; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
};
