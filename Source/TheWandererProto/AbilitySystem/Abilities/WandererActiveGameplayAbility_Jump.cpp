//  


#include "WandererActiveGameplayAbility_Jump.h"

#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"

UWandererActiveGameplayAbility_Jump::UWandererActiveGameplayAbility_Jump()
	: Super(WandererGameplayTags::InputTag_Jump)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Combat);
}

bool UWandererActiveGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!ActorInfo || !ActorInfo->AvatarActor.IsValid()) return false;

	const AWandererCharacter* WandererCharacter = Cast<AWandererCharacter>(ActorInfo->AvatarActor.Get());
	check(WandererCharacter);
	
	if (!WandererCharacter->CanJump()) return false;

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CharacterJumpStart();
}

void UWandererActiveGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Jump Ended")));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Jump::CharacterJumpStart()
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (!Character->bPressedJump)
		{
			Character->UnCrouch();
			Character->Jump();
		}
	}
}

void UWandererActiveGameplayAbility_Jump::CharacterJumpStop()
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (Character->bPressedJump)
		{
			Character->StopJumping();
		}
	}
}



