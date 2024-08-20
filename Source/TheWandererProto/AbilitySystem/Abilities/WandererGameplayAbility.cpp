//  


#include "AbilitySystem/Abilities/WandererGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Animation/WandererAnimInstance.h"

UWandererGameplayAbility::UWandererGameplayAbility()
{
	ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Finisher);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Dead);
}

void UWandererGameplayAbility::AddLooseTagToOwner(const FGameplayTag& GameplayTag) const
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(GameplayTag);
}

void UWandererGameplayAbility::RemoveLooseTagFromOwner(const FGameplayTag& GameplayTag) const
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(GameplayTag);
}

bool UWandererGameplayAbility::DoesOwnerHaveTag(const FGameplayTag& GameplayTag) const
{
	return GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GameplayTag);
}

UAnimMontage* UWandererGameplayAbility::GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontageForTag(GameplayTag);
}

UWandererMontagePair* UWandererGameplayAbility::GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const
{
	return CastChecked<UWandererAnimInstance>(GetActorInfo().GetAnimInstance())->GetMatchingMontagePairForTag(GameplayTag);
}
