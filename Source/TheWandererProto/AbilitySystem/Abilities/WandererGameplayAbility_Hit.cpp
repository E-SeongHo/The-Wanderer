//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Hit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/Component/WandererCombatComponent.h"

UWandererGameplayAbility_Hit::UWandererGameplayAbility_Hit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
	
	AbilityTags.AddTag(WandererGameplayTags::Ability_Hit);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Dead);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_Hit);
	
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
}

void UWandererGameplayAbility_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	
	// hit can cause knockback or not. logic can be added here
	UActionTagWrapper* ActionTagWrapper = NewObject<UActionTagWrapper>();
	ActionTagWrapper->ActionTag = WandererGameplayTags::ActionTag_Hit;
	
	FGameplayEventData EventData;
	EventData.OptionalObject = ActionTagWrapper;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), WandererGameplayTags::Event_Combat_KnockBack, EventData);

	check(DoesOwnerHaveTag(WandererGameplayTags::State_KnockBack));
	UAbilityTask_WaitGameplayTagRemoved* WaitKnockBackEnd = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, WandererGameplayTags::State_KnockBack);
	WaitKnockBackEnd->Removed.AddDynamic(this, &UWandererGameplayAbility_Hit::OnKnockBackEnded);
	WaitKnockBackEnd->ReadyForActivation();
}

void UWandererGameplayAbility_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(!ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		CastChecked<AWandererBaseCharacter>(ActorInfo->AvatarActor)->GetCombatComponent()->StartCombat();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UWandererGameplayAbility_Hit::CanRetrigger() const
{
	check(bRetriggerInstancedAbility);
	return bRetriggerInstancedAbility;
}

void UWandererGameplayAbility_Hit::OnKnockBackEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererGameplayAbility_Hit::OnRecoveryAvailable(FGameplayEventData Payload)
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_CanRecovery);
}
