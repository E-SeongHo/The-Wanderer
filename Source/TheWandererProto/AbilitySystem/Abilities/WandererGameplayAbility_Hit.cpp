//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Hit.h"

#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UWandererGameplayAbility_Hit::UWandererGameplayAbility_Hit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(WandererGameplayTags::Ability_Hit);
}

void UWandererGameplayAbility_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Generate AbilityTask : Play Montage
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Hit React"), HitReactAnims[FMath::RandRange(0, HitReactAnims.Num()-1)]);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Hit::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation();
}

void UWandererGameplayAbility_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererGameplayAbility_Hit::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
