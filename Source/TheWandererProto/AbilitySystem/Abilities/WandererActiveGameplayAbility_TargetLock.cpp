//  


#include "WandererActiveGameplayAbility_TargetLock.h"

#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystem/Effects/WandererGameplayEffect.h"
#include "Camera/CameraComponent.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_TargetLock::UWandererActiveGameplayAbility_TargetLock()
	: Super(WandererGameplayTags::InputTag_TargetLock)
{
	ActivationOwnedTags.AddTag(WandererGameplayTags::State_Combat_TargetLock);
}

bool UWandererActiveGameplayAbility_TargetLock::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AWandererBaseCharacter* Indicator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	check(Indicator);
	
	AWandererBaseCharacter* Target = FindTarget(Indicator);
	if(Target)
	{
		Indicator->GetCombatComponent()->LockOnTarget(Target);
		Indicator->GetCharacterMovement()->bOrientRotationToMovement = false;
		Indicator->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		Indicator->GetCombatComponent()->OnTargetLost.AddDynamic(this, &UWandererActiveGameplayAbility_TargetLock::OnTargetLost);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UWandererActiveGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("End Locking On")));

	const AWandererBaseCharacter* Indicator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	Indicator->GetCombatComponent()->LockOnTarget(nullptr);
	Indicator->GetCharacterMovement()->bOrientRotationToMovement = true;
	Indicator->GetCharacterMovement()->bUseControllerDesiredRotation = false; 

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_TargetLock::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Lock Pressed")));

	if(IsActive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UWandererActiveGameplayAbility_TargetLock::OnTargetLost()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

AWandererBaseCharacter* UWandererActiveGameplayAbility_TargetLock::FindTarget(const AWandererBaseCharacter* SrcCharacter)
{
	const UWandererCombatComponent* CombatComponent = SrcCharacter->GetCombatComponent();

	const FVector Location = SrcCharacter->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	const FVector CameraDirection = SrcCharacter->GetControlRotation().Vector();
	const float TraceRadius = SrcCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FHitResult HitResult;
	const bool bHit = WandererUtils::SphereTrace(HitResult, SrcCharacter, Location, Location + CameraDirection * CombatComponent->GetLockOnDistance(), TraceRadius, ECC_GameTraceChannel1);
	if(bHit)
	{
		AWandererBaseCharacter* TargetCharaceter = Cast<AWandererBaseCharacter>(HitResult.GetActor());
		check(TargetCharaceter); // TODO : make something more specific : e.g) Enemy Interface?

		return TargetCharaceter;
	}

	return nullptr;	
}
