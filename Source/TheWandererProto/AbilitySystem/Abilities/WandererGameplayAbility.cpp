//  


#include "AbilitySystem/Abilities/WandererGameplayAbility.h"

#include "Animation/WandererAnimInstance.h"

UAnimMontage* UWandererGameplayAbility::GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontageForTag(GameplayTag);
}
