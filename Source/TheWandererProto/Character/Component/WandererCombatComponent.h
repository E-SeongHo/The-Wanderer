//  

#pragma once

#include "CoreMinimal.h"
#include "WandererActorComponent.h"
#include "WandererCombatComponent.generated.h"

class AWandererBaseCharacter;
class AWandererWeapon;

DECLARE_MULTICAST_DELEGATE(FWandererCombat_Combat);
DECLARE_MULTICAST_DELEGATE_OneParam(FWandererCombat_Target, AWandererBaseCharacter*);

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererCombatComponent : public UWandererActorComponent
{
	GENERATED_BODY()

public:
	UWandererCombatComponent();

	//AWandererWeapon* GetWeapon() const { return Weapon.Get(); }
	AWandererBaseCharacter* GetCombatTarget() const { return CombatTarget.Get(); }
	bool IsTargetInAttackRange() const;
	bool IsTargetInDashRange() const;

	bool CanDashTo(const AWandererBaseCharacter* DashTarget) const;
	bool CanFinishTarget() const;
	
	void SetCombatTarget(AWandererBaseCharacter* InTarget);
	void StartCombat();
	void EndCombat();
	void Runaway();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	FWandererCombat_Combat OnCombatStarted;
	
	FWandererCombat_Combat OnCombatEnded;

	FWandererCombat_Target OnTargetChanged;

	// Hard lock (GA_TargetLock) needs it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bUseActorDesiredControlRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CombatAcceptanceRadius = 700.0f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<AWandererBaseCharacter> CombatTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true")) 
	float AttackAvailableDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true")) 
	float DashAvailableDistance = 500.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CombatExitDelay = 3.0f;
	
	bool bIsInCombat = false;
};
