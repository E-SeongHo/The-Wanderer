//  


#include "WandererActiveGameplayAbility_TargetLock.h"

#include "WandererGameplayTags.h"

UWandererActiveGameplayAbility_TargetLock::UWandererActiveGameplayAbility_TargetLock()
	: Super(WandererGameplayTags::InputTag_TargetLock)
{
	
}

bool UWandererActiveGameplayAbility_TargetLock::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
