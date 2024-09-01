//  


#include "WandererActiveGameplayAbility_Dodge.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_Dodge::UWandererActiveGameplayAbility_Dodge()
	: Super(WandererGameplayTags::InputTag_Dodge)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;

	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Draw);
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Combat);
	
	//ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Hit);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Avoid);
	
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Parry);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Hit);
}

bool UWandererActiveGameplayAbility_Dodge::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* OwnerASC = ActorInfo->AbilitySystemComponent.Get();
	if(OwnerASC->HasMatchingGameplayTag(WandererGameplayTags::Ability_Hit))
	{
		return OwnerASC->HasMatchingGameplayTag(WandererGameplayTags::State_CanRecovery);
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("dodge")));	

	const FVector MovementInput = Cast<APawn>(ActorInfo->AvatarActor)->GetLastMovementInputVector();

	UAnimMontage* MontageToPlay = nullptr;
	switch(WandererUtils::EvaluateDirectionRelativeToActor(ActorInfo->AvatarActor.Get(), MovementInput))
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