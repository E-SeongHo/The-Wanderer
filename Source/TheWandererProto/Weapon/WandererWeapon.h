//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "WandererWeapon.generated.h"

class AWandererBaseCharacter;

UCLASS()
class THEWANDERERPROTO_API AWandererWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWandererWeapon();
	UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh.Get(); }

	void InitializeWithOwner(AWandererBaseCharacter* InWeaponOwner);
	
	// TODO : Think about another options...
	// 1) define class RangedWeapon
	// 2) define interface Traceable
	// 3) Grant Ability to weapon  
	virtual bool Trace(FHitResult& OutHit) { check(false); return false; }
	USoundBase* GetTraceSound() const { return TraceSound.Get(); }

public:
	FGameplayTag EquippedTag;
	FGameplayTag DrawnTag;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AWandererBaseCharacter> WeaponOwner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> TraceSound;
};
