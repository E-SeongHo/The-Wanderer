//  


#include "WandererActiveGameplayAbility_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Tasks/WandererAbilityTask_SmoothRotate.h"

UWandererActiveGameplayAbility_Attack::UWandererActiveGameplayAbility_Attack()
	: Super(WandererGameplayTags::InputTag_Attack)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_Attack);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_Attack);

	ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Hit);
	ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Finisher);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Avoid);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_KnockBack);
	
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Draw);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Parry);
}

void UWandererActiveGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	DetermineAttackAction();
	ProcessAttack();
}

void UWandererActiveGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Attack::ProcessAttack()
{
	SoftLock();
	PlayNewMontageTask(GetMatchingMontageForTag(CurrentActionTag));
}

void UWandererActiveGameplayAbility_Attack::SoftLock()
{
	// Attack orientation
	const AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());
	UWandererAbilityTask_SmoothRotate* SmoothRotator = UWandererAbilityTask_SmoothRotate::SmoothRotate(this, Instigator->GetActorRotation(), Instigator->GetControlRotation());
	SmoothRotator->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Attack::PlayNewMontageTask(UAnimMontage* MontageToPlay)
{
	if(CurrentPlayingMontageTask.Get() && CurrentPlayingMontageTask->IsActive())
	{
		CurrentPlayingMontageTask->ExternalCancel();
	}
	
	CurrentPlayingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayMontage"), MontageToPlay);
	CurrentPlayingMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
	CurrentPlayingMontageTask->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Attack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

EWandererAttackResult UWandererActiveGameplayAbility_Attack::EvaluateAttackResult(AWandererBaseCharacter* Target)
{
	if(Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Parry))
	{
		// Attack succeeds if from behind, even if target is parrying
		const FVector TargetForward = Target->GetActorForwardVector();
		const FVector AttackOrientation = GetAvatarActorFromActorInfo()->GetActorForwardVector();
		const float Cos = FVector::DotProduct(TargetForward, AttackOrientation);

		if(Cos < 0.7f) return EWandererAttackResult::Blocked;
	}
	if(Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Avoid))
	{
		return EWandererAttackResult::Miss;
	}
	
	return EWandererAttackResult::Success;
}