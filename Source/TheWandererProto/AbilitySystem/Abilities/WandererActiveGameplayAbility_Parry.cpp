//  


#include "WandererActiveGameplayAbility_Parry.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCharacterMovementComponent.h"

UWandererActiveGameplayAbility_Parry::UWandererActiveGameplayAbility_Parry()
	: Super(WandererGameplayTags::InputTag_Parry)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationOwnedTags.AddTag(WandererGameplayTags::State_Combat_Parry);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Sprint);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Walk);
}

void UWandererActiveGameplayAbility_Parry::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// After the Ready to Parry Animation has completed, character can actually parry the attack

	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Parry Activate")));

	UWandererCharacterMovementComponent* CharacterMovement = CastChecked<UWandererCharacterMovementComponent>(CastChecked<AWandererCharacter>(ActorInfo->AvatarActor)->GetCharacterMovement()); 
	CharacterMovement->StartWalking();
	
	UAbilityTask_WaitGameplayEvent* WaitSomeoneAttack = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Combat_ParryAttack);
	WaitSomeoneAttack->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Parry::OnParrySucceeded);
	WaitSomeoneAttack->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Parry::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UWandererCharacterMovementComponent* CharacterMovement = CastChecked<UWandererCharacterMovementComponent>(CastChecked<AWandererCharacter>(ActorInfo->AvatarActor)->GetCharacterMovement()); 
	CharacterMovement->StopWalking();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Parry::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	check(IsActive());
	EndAbility(Handle, ActorInfo, ActivationInfo, true ,false);
}

void UWandererActiveGameplayAbility_Parry::OnParrySucceeded(FGameplayEventData Payload)
{
	// get direction of the instigator and play corresponding parry animation   
	//const FVector AttackFrom = Payload.Instigator->GetActorLocation();

	UAbilityTask_PlayMontageAndWait* PlayMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Parry"), ParryAnimFromRight);
	PlayMontage->ReadyForActivation();
}

