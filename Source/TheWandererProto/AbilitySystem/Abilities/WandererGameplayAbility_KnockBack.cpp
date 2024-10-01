//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_KnockBack.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayAbility_Die.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UWandererGameplayAbility_KnockBack::UWandererGameplayAbility_KnockBack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
	
	AbilityTags.AddTag(WandererGameplayTags::Ability_KnockBack);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_KnockBack);
	ActivationOwnedTags.AddTag(WandererGameplayTags::State_KnockBack);
	
	//CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = WandererGameplayTags::Event_Combat_KnockBack;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UWandererGameplayAbility_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// KnockBack is triggered only by the events, and the event dispatcher is responsible for delivering the ActionTag.
	// Since KnockBack can happen in various situations, different animation types are required depending on the situation.
	check(TriggerEventData);
	const FGameplayTag ActionTag = CastChecked<UActionTagWrapper>(TriggerEventData->OptionalObject)->ActionTag;
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("KnockBack"), GetMatchingMontageForTag(ActionTag));
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_KnockBack::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation(); 

	UAbilityTask_WaitGameplayEvent* WaitRecoveryAvailable = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_Land);
	WaitRecoveryAvailable->EventReceived.AddDynamic(this, &UWandererGameplayAbility_KnockBack::OnRecoveryAvailable);
	WaitRecoveryAvailable->ReadyForActivation();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UWandererGameplayAbility_KnockBack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_CanRecovery);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UWandererGameplayAbility_KnockBack::CanRetrigger() const
{
	check(bRetriggerInstancedAbility);
	return bRetriggerInstancedAbility;
}

void UWandererGameplayAbility_KnockBack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererGameplayAbility_KnockBack::OnRecoveryAvailable(FGameplayEventData Payload)
{
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_CanRecovery);
}
