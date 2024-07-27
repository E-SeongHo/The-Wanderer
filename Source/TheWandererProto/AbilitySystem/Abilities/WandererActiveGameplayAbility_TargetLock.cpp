//  


#include "WandererActiveGameplayAbility_TargetLock.h"

#include "WandererGameplayTags.h"
#include "AbilitySystem/Effects/WandererGameplayEffect.h"
#include "Camera/CameraComponent.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tasks/WandererAbilityTask.h"
#include "Tasks/WandererAbilityTask_RepeatUntil.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_TargetLock::UWandererActiveGameplayAbility_TargetLock()
	: Super(WandererGameplayTags::InputTag_TargetLock)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Combat);
	ActivationOwnedTags.AddTag(WandererGameplayTags::State_Combat_TargetLock);
}

bool UWandererActiveGameplayAbility_TargetLock::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(ActorInfo->AvatarActor);
	check(Instigator);
	
	Target = FindTarget(Instigator);
	if(Target.Get())
	{
		Instigator->GetCombatComponent()->bUseActorDesiredControlRotation = true;
		Instigator->GetCombatComponent()->SetCombatTarget(Target.Get());
		
		// target monitoring 
		UWandererAbilityTask_RepeatUntil* MonitorTargetTask = UWandererAbilityTask_RepeatUntil::RepeatAction(this, 1.0f);
		MonitorTargetTask->OnPerformAction.AddDynamic(this, &UWandererActiveGameplayAbility_TargetLock::MonitorTarget);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UWandererActiveGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("End Locking On")));

	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(ActorInfo->AvatarActor);
	Instigator->GetCombatComponent()->bUseActorDesiredControlRotation = false;
	// if still in combat, GA_AutoTarget will find another target immediately
	Instigator->GetCombatComponent()->SetCombatTarget(nullptr);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_TargetLock::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Lock Pressed")));

	check(IsActive())
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_TargetLock::OnTargetLost()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UWandererActiveGameplayAbility_TargetLock::MonitorTarget()
{
	const AWandererCharacter* Instigator = Cast<AWandererCharacter>(GetAvatarActorFromActorInfo());

	// while this GA is activating, combat component's combat target always same with this GA's monitoring target   
	check(Target == Instigator->GetCombatComponent()->GetCombatTarget());

	const float DistanceToTarget = FVector::Distance(Instigator->GetActorLocation(), Target->GetActorLocation());

	// TODO: Check Target is alive ? 
	
	if(DistanceToTarget > Instigator->GetCombatComponent()->CombatAcceptanceRadius)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

AWandererBaseCharacter* UWandererActiveGameplayAbility_TargetLock::FindTarget(const AWandererBaseCharacter* SrcCharacter)
{
	const UWandererCombatComponent* CombatComponent = SrcCharacter->GetCombatComponent();

	const FVector Location = SrcCharacter->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
	const FVector CameraDirection = SrcCharacter->GetControlRotation().Vector();
	const float TraceRadius = SrcCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FHitResult HitResult;
	const bool bHit = WandererUtils::SphereTrace(HitResult, SrcCharacter, Location, Location + CameraDirection * CombatComponent->CombatAcceptanceRadius, TraceRadius, ECC_GameTraceChannel1);
	if(bHit)
	{
		AWandererBaseCharacter* TargetCharaceter = Cast<AWandererBaseCharacter>(HitResult.GetActor());
		check(TargetCharaceter);
		
		return TargetCharaceter;
	}

	return nullptr;	
}
