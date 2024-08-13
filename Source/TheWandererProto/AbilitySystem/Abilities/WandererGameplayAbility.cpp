//  


#include "AbilitySystem/Abilities/WandererGameplayAbility.h"

#include "WandererGameplayTags.h"
#include "Animation/WandererAnimInstance.h"

UWandererGameplayAbility::UWandererGameplayAbility()
{
	ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Finisher);
}

UAnimMontage* UWandererGameplayAbility::GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontageForTag(GameplayTag);
}

UWandererMontagePair* UWandererGameplayAbility::GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontagePairForTag(GameplayTag);
}
