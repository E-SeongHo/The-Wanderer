//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_DrawWeapon.generated.h"

/**
 * 
 */
UCLASS()
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
};
