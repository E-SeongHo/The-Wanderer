//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Die.h"

#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/WandererBaseCharacter.h"

UWandererGameplayAbility_Die::UWandererGameplayAbility_Die()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(WandererGameplayTags::Ability_Die);
}

void UWandererGameplayAbility_Die::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Generate AbilityTask : Play Montage
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Hit React"), DieAnims[FMath::RandRange(0, DieAnims.Num()-1)]);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Die::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation();

	AWandererBaseCharacter* WandererCharacter = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
	if(WandererCharacter)
	{
		WandererCharacter->Die();
	}
}

void UWandererGameplayAbility_Die::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererGameplayAbility_Die::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
