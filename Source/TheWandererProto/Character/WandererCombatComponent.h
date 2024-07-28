//  

#pragma once

#include "CoreMinimal.h"
#include "Character/WandererActorComponent.h"
#include "WandererCombatComponent.generated.h"


class UNiagaraSystem;
class AWandererBaseCharacter;
class AWandererWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWandererCombat_Combat);
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
	
	void EquipWeapon(AWandererWeapon* NewWeapon, FName SocketName);
	void AttachWeaponMeshToSocket(FName SocketName);

	void SetCombatTarget(AWandererBaseCharacter* InTarget);
	void StartCombat();
	void EndCombat();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	// GA can add it's own logic with delegates
	UPROPERTY()
	FWandererCombat_Combat OnCombatStarted;

	UPROPERTY()
	FWandererCombat_Combat OnCombatEnded;

public:
	// Hard lock (GA_TargetLock) needs it
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bUseActorDesiredControlRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CombatAcceptanceRadius = 2000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CombatExitDelay = 3.0f;
	
private:
	// Combat component owns weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWandererWeapon> Weapon; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<AWandererBaseCharacter> CombatTarget;

	bool bIsInCombat = false;
};
