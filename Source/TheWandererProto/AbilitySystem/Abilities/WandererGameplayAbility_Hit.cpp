//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Hit.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
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

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Hit React"), GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Hit));
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Hit::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UWandererGameplayAbility_Hit::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitRecoveryAvailable = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_Land);
	WaitRecoveryAvailable->EventReceived.AddDynamic(this, &UWandererGameplayAbility_Hit::OnRecoveryAvailable);
	WaitRecoveryAvailable->ReadyForActivation();
}

void UWandererGameplayAbility_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(!ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		CastChecked<AWandererBaseCharacter>(ActorInfo->AvatarActor)->GetCombatComponent()->StartCombat();
	}

	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_CanRecovery);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UWandererGameplayAbility_Hit::CanRetrigger() const
{
	check(bRetriggerInstancedAbility);
	return bRetriggerInstancedAbility;
}

void UWandererGameplayAbility_Hit::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererGameplayAbility_Hit::OnRecoveryAvailable(FGameplayEventData Payload)
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_CanRecovery);
}
