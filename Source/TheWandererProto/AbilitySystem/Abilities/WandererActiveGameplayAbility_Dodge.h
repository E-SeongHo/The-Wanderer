//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Dodge : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void CallEndAbility();	

	UFUNCTION()
	void RemoveAvoidTag(FGameplayEventData Payload);
};

