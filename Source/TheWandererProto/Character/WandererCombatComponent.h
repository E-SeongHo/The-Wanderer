//  

#pragma once

#include "CoreMinimal.h"
#include "Character/WandererActorComponent.h"
#include "WandererCombatComponent.generated.h"

class AWandererBaseCharacter;
class AWandererWeapon;

USTRUCT(BlueprintType)
struct FWandererWeaponConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "AWandererWeapon"))
	TSubclassOf<AWandererWeapon> WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DrawSocket;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SheathSocket;
};


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

	virtual void AssignAbilitySystemComponent(UAbilitySystemComponent* OwnerASC) override;
	
	AWandererWeapon* GetWeapon() const { return Weapon.Get(); }
	AWandererBaseCharacter* GetCombatTarget() const { return CombatTarget.Get(); }
	bool IsTargetInAttackRange() const;

	void AttachWeaponToDrawSocket() const;
	void AttachWeaponToSheathSocket() const;

	bool CanFinishTarget() const;
	
	void SetCombatTarget(AWandererBaseCharacter* InTarget);
	void StartCombat();
	void EndCombat();
	void Runaway();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void EquipWeapon();
	
public:
	FWandererCombat_Combat OnCombatStarted;
	
	FWandererCombat_Combat OnCombatEnded;

	FWandererCombat_Target OnTargetChanged;

	// Hard lock (GA_TargetLock) needs it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bUseActorDesiredControlRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CombatAcceptanceRadius = 700.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CombatExitDelay = 3.0f;
	
private:
	// Set in BluePrint, it can be expanded to array ( can have multiple weapons )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FWandererWeaponConfig WeaponConfig;
	
	// Combat component owns weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWandererWeapon> Weapon; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<AWandererBaseCharacter> CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true")) 
	float AttackAvailableDistance = 300.0f;
	
	bool bIsInCombat = false;
};
