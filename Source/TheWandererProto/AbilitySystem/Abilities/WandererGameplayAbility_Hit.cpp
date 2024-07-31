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
	
	AbilityTags.AddTag(WandererGameplayTags::Ability_Hit);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_Hit);
	
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
}

void UWandererGameplayAbility_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Generate AbilityTask : Play Montage
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSounds[FMath::RandRange(0, HitSounds.Num()-1)], Instigator->GetActorLocation());

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Hit React"), HitReactAnims[FMath::RandRange(0, HitReactAnims.Num()-1)]);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Hit::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation();
}

void UWandererGameplayAbility_Hit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(!ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		CastChecked<AWandererBaseCharacter>(ActorInfo->AvatarActor)->GetCombatComponent()->StartCombat();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererGameplayAbility_Hit::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
