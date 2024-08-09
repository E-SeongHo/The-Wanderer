//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Hit.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Kismet/GameplayStatics.h"

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
	// Generate AbilityTask : Play Montage
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Hit React"), GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Hit));

	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Hit::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UWandererGameplayAbility_Hit::OnMontageCompleted);
	
	PlayMontageTask->ReadyForActivation();

	if(!ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		CastChecked<AWandererBaseCharacter>(ActorInfo->AvatarActor)->GetCombatComponent()->StartCombat();
	}
}

void UWandererGameplayAbility_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererGameplayAbility_Hit::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
