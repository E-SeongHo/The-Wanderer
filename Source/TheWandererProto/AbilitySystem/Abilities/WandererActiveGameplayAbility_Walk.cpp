//  


#include "WandererActiveGameplayAbility_Walk.h"

#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCharacterMovementComponent.h"

UWandererActiveGameplayAbility_Walk::UWandererActiveGameplayAbility_Walk()
	:Super(WandererGameplayTags::InputTag_Walk)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_Walk);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Combat);
}

void UWandererActiveGameplayAbility_Walk::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(ActorInfo->AvatarActor);
	UWandererCharacterMovementComponent* CharacterMovement = Cast<UWandererCharacterMovementComponent>(Instigator->GetCharacterMovement());

	CharacterMovement->StartWalking();
}

void UWandererActiveGameplayAbility_Walk::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(ActorInfo->AvatarActor);
	UWandererCharacterMovementComponent* CharacterMovement = Cast<UWandererCharacterMovementComponent>(Instigator->GetCharacterMovement());

	CharacterMovement->StopWalking();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Walk::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	check(IsActive());
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
