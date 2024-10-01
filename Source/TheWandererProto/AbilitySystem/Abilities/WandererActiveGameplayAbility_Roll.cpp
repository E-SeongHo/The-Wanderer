//  


#include "WandererActiveGameplayAbility_Roll.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_Roll::UWandererActiveGameplayAbility_Roll()
	: Super(WandererGameplayTags::InputTag_Roll)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;

	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Draw);
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Combat);
	
	//ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Hit); 
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Avoid);

	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_KnockBack);
}

bool UWandererActiveGameplayAbility_Roll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* OwnerASC = ActorInfo->AbilitySystemComponent.Get();
	if(OwnerASC->HasMatchingGameplayTag(WandererGameplayTags::State_KnockBack))
	{
		return OwnerASC->HasMatchingGameplayTag(WandererGameplayTags::State_CanRecovery);
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FVector MovementInput = Cast<APawn>(ActorInfo->AvatarActor)->GetLastMovementInputVector();

	UAnimMontage* MontageToPlay = nullptr;
	switch(WandererUtils::EvaluateDirectionRelativeToActor(ActorInfo->AvatarActor.Get(), MovementInput))
	{
	case EDirection::Forward:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Roll_Front);
		break;
	case EDirection::Backward:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Roll_Back);
		break;
	case EDirection::Right:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Roll_Right);
		break;
	case EDirection::Left:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Roll_Left);
		break;
	default:
		MontageToPlay = GetMatchingMontageForTag(WandererGameplayTags::ActionTag_Roll_Back);
		break;
	}

	check(MontageToPlay);
	UAbilityTask_PlayMontageAndWait* PlayDodgeMontage = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Roll"), MontageToPlay);
	PlayDodgeMontage->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Roll::CallEndAbility);
	PlayDodgeMontage->OnInterrupted.AddDynamic(this, &UWandererActiveGameplayAbility_Roll::CallEndAbility);
	PlayDodgeMontage->ReadyForActivation();

	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_Avoid);
	UAbilityTask_WaitGameplayEvent* WaitEndAvoidance = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_Land);
	WaitEndAvoidance->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Roll::RemoveAvoidTag);
	WaitEndAvoidance->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_Avoid);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UWandererActiveGameplayAbility_Roll::CanRetrigger() const
{
	check(bRetriggerInstancedAbility);
	return bRetriggerInstancedAbility;
}

void UWandererActiveGameplayAbility_Roll::CallEndAbility()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Roll::RemoveAvoidTag(FGameplayEventData Payload)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_Avoid);
}