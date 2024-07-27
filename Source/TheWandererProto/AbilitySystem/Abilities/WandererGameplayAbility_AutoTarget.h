//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "WandererGameplayAbility_AutoTarget.generated.h"

class AGameplayAbilityWorldReticle;
class AWandererBaseCharacter;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayAbility_AutoTarget : public UWandererGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWandererGameplayAbility_AutoTarget();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void SearchTargetAvailable();
};
