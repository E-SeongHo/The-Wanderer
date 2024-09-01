//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility_Attack.h"
#include "WandererActiveGameplayAbility_Melee.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Melee : public UWandererActiveGameplayAbility_Attack
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_Melee();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanRetrigger() const override;
	
protected:
	virtual void DetermineAttackAction() override;

	virtual void SoftLock() override;
	
	UFUNCTION()
	void OnWeaponTraceStart();

	UFUNCTION()
	void OnWeaponTrace();
	
	void SetComboAvailable(bool bIsAvailable);
};
