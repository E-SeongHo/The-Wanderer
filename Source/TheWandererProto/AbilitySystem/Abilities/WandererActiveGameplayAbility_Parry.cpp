//  


#include "WandererActiveGameplayAbility_Parry.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/WandererCharacter.h"
#include "Character/Component/WandererCharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_Parry::UWandererActiveGameplayAbility_Parry()
	: Super(WandererGameplayTags::InputTag_Parry)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_Parry);
	ActivationOwnedTags.AddTag(WandererGameplayTags::State_Parry);

	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Draw);
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Combat);

	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Avoid);
	ActivationBlockedTags.AddTag(WandererGameplayTags::Ability_Hit);
	
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);
}

bool UWandererActiveGameplayAbility_Parry::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* OwnerASC = ActorInfo->AbilitySystemComponent.Get();
	if(OwnerASC->HasMatchingGameplayTag(WandererGameplayTags::Ability_Attack))
	{
		return OwnerASC->HasMatchingGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_Parry::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CheckCooldown(Handle, ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Parry Activate")));

	UWandererCharacterMovementComponent* CharacterMovement = CastChecked<UWandererCharacterMovementComponent>(CastChecked<AWandererBaseCharacter>(ActorInfo->AvatarActor)->GetCharacterMovement()); 
	CharacterMovement->StartWalking();
	
	UAbilityTask_WaitGameplayEvent* WaitParrySucceeded = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Combat_ParrySucceeded);
	WaitParrySucceeded->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Parry::OnParrySucceeded);
	WaitParrySucceeded->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitParryFailed = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Combat_ParryFailed);
	WaitParryFailed->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Parry::OnParryFailed);
	WaitParryFailed->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Parry::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UWandererCharacterMovementComponent* CharacterMovement = CastChecked<UWandererCharacterMovementComponent>(CastChecked<AWandererBaseCharacter>(ActorInfo->AvatarActor)->GetCharacterMovement()); 
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
	// Fail to parry if the stamina is not enough
	if(!CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		ApplyCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		OnParryFailed(Payload);
		return;
	}
	
	// Get direction of the instigator and play corresponding parry animation
	if(!DoesOwnerHaveTag(WandererGameplayTags::State_Draw_Shield))
	{
		const EDirection AttackCameDirection = WandererUtils::EvaluateDirectionRelativeToActor(GetAvatarActorFromActorInfo(), -Payload.Instigator->GetActorForwardVector());
		check(AttackCameDirection != EDirection::Backward);

		FGameplayTag ActionTag;
		switch(AttackCameDirection)
		{
		case EDirection::Forward:
			ActionTag = WandererGameplayTags::ActionTag_Parry_Front;
			break;
		case EDirection::Left:
			ActionTag = WandererGameplayTags::ActionTag_Parry_Left;
			break;
		case EDirection::Right:
			ActionTag = WandererGameplayTags::ActionTag_Parry_Right;
			break;
		default:
			check(false);
		}
		
		TriggerKnockBack(ActionTag);
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ParrySounds[FMath::RandRange(0, ParrySounds.Num()-1)], GetAvatarActorFromActorInfo()->GetActorLocation());
}

void UWandererActiveGameplayAbility_Parry::OnParryFailed(FGameplayEventData Payload)
{
	TriggerKnockBack(WandererGameplayTags::ActionTag_Parry_Rebound);
	
	ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ParrySounds[FMath::RandRange(0, ParrySounds.Num()-1)], GetAvatarActorFromActorInfo()->GetActorLocation());
	ReleaseParry();
}

void UWandererActiveGameplayAbility_Parry::ReleaseParry()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UWandererActiveGameplayAbility_Parry::TriggerKnockBack(const FGameplayTag& ActionTag) const
{
	UActionTagWrapper* ActionTagWrapper = NewObject<UActionTagWrapper>();
	ActionTagWrapper->ActionTag = ActionTag;
	
	FGameplayEventData EventData;
	EventData.OptionalObject = ActionTagWrapper;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), WandererGameplayTags::Event_Combat_KnockBack, EventData);
}
