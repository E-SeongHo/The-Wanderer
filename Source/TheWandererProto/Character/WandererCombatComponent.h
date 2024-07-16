//  

#pragma once

#include "CoreMinimal.h"
#include "Character/WandererActorComponent.h"
#include "WandererCombatComponent.generated.h"

class AWandererBaseCharacter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWandererCombat_TargetLost);

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererCombatComponent : public UWandererActorComponent
{
	GENERATED_BODY()

public:
	UWandererCombatComponent();

	float GetLockOnDistance() const { return LockOnDistance; }
	void LockOnTarget(AWandererBaseCharacter* TargetToLock);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	FWandererCombat_TargetLost OnTargetLost;

private:
	bool IsLockingOn() const;
	
private:
	UPROPERTY() // maybe blueprint need to see it later
	TObjectPtr<AWandererBaseCharacter> LockTarget;

	float LockOnDistance = 2000.0f;
};
