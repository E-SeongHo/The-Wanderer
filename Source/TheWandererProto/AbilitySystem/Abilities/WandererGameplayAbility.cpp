//  


#include "AbilitySystem/Abilities/WandererGameplayAbility.h"

#include "Animation/WandererAnimInstance.h"
#include "Animation/WandererAnimMontageConfig.h"

UAnimMontage* UWandererGameplayAbility::GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontageForTag(GameplayTag);
}

UWandererMontagePair* UWandererGameplayAbility::GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontagePairForTag(GameplayTag);
}
