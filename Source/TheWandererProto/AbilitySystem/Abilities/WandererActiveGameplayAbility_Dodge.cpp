//  


#include "WandererActiveGameplayAbility_Dodge.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_Dodge::UWandererActiveGameplayAbility_Dodge()
	: Super(WandererGameplayTags::InputTag_Dodge)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Draw);
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Combat);
	
	ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Hit);
	
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
}

void UWandererActiveGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const FVector Velocity = ActorInfo->AvatarActor->GetVelocity(); 
	if(Velocity.Size2D() < 3.0f)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

	// Use actor's velocity as desired direction
	UAnimMontage* MontageToPlay = nullptr;
	switch(WandererUtils::EvaluateDirectionRelativeToActor(ActorInfo->AvatarActor.Get(), Velocity.GetSafeNormal2D()))
	{
	case EDirection::Forward:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Dodge_Front);
		break;
	case EDirection::Backward:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Dodge_Back);
		break;
	case EDirection::Right:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Dodge_Right);
		break;
	case EDirection::Left:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Dodge_Left);
		break;
	default:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Dodge_Back);
		break;
	}

	check(MontageToPlay);
	UAbilityTask_PlayMontageAndWait* PlayDodgeMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Dodge"), MontageToPlay);
	PlayDodgeMontage->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Dodge::CallEndAbility);
	PlayDodgeMontage->OnInterrupted.AddDynamic(this, &UWandererActiveGameplayAbility_Dodge::CallEndAbility);
	PlayDodgeMontage->ReadyForActivation();

	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_Avoid);
	UAbilityTask_WaitGameplayEvent* WaitEndAvoidance = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_Land);
	WaitEndAvoidance->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Dodge::RemoveAvoidTag);
	WaitEndAvoidance->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_Avoid);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Dodge::CallEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Dodge::RemoveAvoidTag(FGameplayEventData Payload)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_Avoid);
}