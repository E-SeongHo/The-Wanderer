//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WandererEquipment.h"
#include "GameFramework/Actor.h"
#include "WandererWeapon.generated.h"

UENUM()
enum class EWandererWeaponHandType
{
	OneHand,
	TwoHand,
};

UCLASS(Abstract)
class THEWANDERERPROTO_API AWandererWeapon : public AWandererEquipment
{
	GENERATED_BODY()
	
public:	
	AWandererWeapon();

	virtual bool Trace(FHitResult& OutHit) PURE_VIRTUAL(AWandererWeapon::Trace, return false;);
	USoundBase* GetTraceSound() const { return TraceSound.Get(); }

	virtual void OnDraw() override;
	virtual void OnSheath() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> TraceSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	EWandererWeaponHandType HandType; 
};
