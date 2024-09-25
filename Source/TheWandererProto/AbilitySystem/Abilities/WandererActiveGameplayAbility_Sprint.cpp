//  


#include "WandererActiveGameplayAbility_Sprint.h"

#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Character/Component/WandererCharacterMovementComponent.h"
#include "Tasks/WandererAbilityTask_RepeatUntil.h"

UWandererActiveGameplayAbility_Sprint::UWandererActiveGameplayAbility_Sprint()
	:Super(WandererGameplayTags::InputTag_Sprint)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_Sprint);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_Sprint);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Combat);
}

void UWandererActiveGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(ActorInfo->AvatarActor);
	UWandererCharacterMovementComponent* CharacterMovement = Cast<UWandererCharacterMovementComponent>(Instigator->GetCharacterMovement());

	CharacterMovement->StartSprinting();
	UWandererAbilityTask_RepeatUntil* DrainStamina = UWandererAbilityTask_RepeatUntil::RepeatAction(this, 0.1f);
	DrainStamina->OnPerformAction.AddDynamic(this, &UWandererActiveGameplayAbility_Sprint::DrainStamina);
	DrainStamina->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(ActorInfo->AvatarActor);
	UWandererCharacterMovementComponent* CharacterMovement = Cast<UWandererCharacterMovementComponent>(Instigator->GetCharacterMovement());

	CharacterMovement->StopSprinting();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	check(IsActive());
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Sprint::DrainStamina()
{
	if(!CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
