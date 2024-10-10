//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "Character/Component/WandererEquipmentComponent.h"
#include "WandererActiveGameplayAbility_DrawWeapon.generated.h"

/**
 * This class should be inherited in BP to enable weapon functionality.
 * Set up WeaponSlot to manage equipped weapons (extend slots as needed for more weapons).
 * Bind input actions to trigger the corresponding weapon abilities.
 */
UCLASS(Abstract)
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_DrawWeapon : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_DrawWeapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnDraw(FGameplayEventData Payload);

	UFUNCTION()
	void SheathAndEndAbility();
	
	UFUNCTION()
	void OnSheath(FGameplayEventData Payload);

	UFUNCTION()
	void OnSheathCompleted();

	UFUNCTION()
	void CreateWaitCombatEndTask();

private:
	UPROPERTY(EditDefaultsOnly)
	EWandererEquipmentSlot WeaponSlot = EWandererEquipmentSlot::Weapon1;

	bool bIsSheathing = false;
};
