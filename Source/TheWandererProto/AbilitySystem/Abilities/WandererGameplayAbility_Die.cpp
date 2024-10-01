//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Die.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Animation/WandererAnimMontageConfig.h"
#include "Character/WandererBaseCharacter.h"

UWandererGameplayAbility_Die::UWandererGameplayAbility_Die()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(WandererGameplayTags::Ability_Die);
	
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Dead);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = WandererGameplayTags::Event_Combat_Victim;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UWandererGameplayAbility_Die::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ActorInfo->AbilitySystemComponent->CancelAbilities(nullptr, nullptr, this);
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_Dead);

	UAnimMontage* MontageToPlay = nullptr;
	if(TriggerEventData)
	{
		MontageToPlay = Cast<UWandererMontagePair>(TriggerEventData->OptionalObject)->Data.VictimMontage;
	}
	else
	{
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Die);
	}

	UAbilityTask_WaitGameplayEvent* WaitVictimDown = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_VictimDown);
	WaitVictimDown->EventReceived.AddDynamic(this, &UWandererGameplayAbility_Die::OnVictimDown);
	WaitVictimDown->ReadyForActivation();
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Die"), MontageToPlay);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Die::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation(); 
}

void UWandererGameplayAbility_Die::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererGameplayAbility_Die::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererGameplayAbility_Die::OnVictimDown(FGameplayEventData Payload)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());
	if(Instigator)
	{
		Instigator->Die();
	}
}
