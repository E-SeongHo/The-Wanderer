//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_AutoTarget.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCharacterMovementComponent.h"
#include "Character/WandererCombatComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tasks/WandererAbilityTask_RepeatUntil.h"
#include "Tasks/WandererAbilityTask_SmoothRotate.h"

UWandererGameplayAbility_AutoTarget::UWandererGameplayAbility_AutoTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_AutoTarget);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_AutoTarget);
	//ActivationRequiredTags.AddTag(WandererGameplayTags::State_Combat);
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = WandererGameplayTags::State_Combat;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagPresent;
	AbilityTriggers.Add(TriggerData);
}

void UWandererGameplayAbility_AutoTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Start Auto Lock")));

	UWandererAbilityTask_RepeatUntil* SearchTarget = UWandererAbilityTask_RepeatUntil::RepeatAction(this, 1.0f);
	SearchTarget->OnPerformAction.AddDynamic(this, &UWandererGameplayAbility_AutoTarget::SearchTargetAvailable);
	SearchTarget->ReadyForActivation();
}

void UWandererGameplayAbility_AutoTarget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("End Auto Lock")));
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererGameplayAbility_AutoTarget::SearchTargetAvailable()
{
	AWandererCharacter* Instigator = Cast<AWandererCharacter>(CurrentActorInfo->AvatarActor);

	// skip finding target since it's already hard locking 
	if(Instigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat_TargetLock))
	{
		return;
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Instigator);
	TArray<FOverlapResult> OutResults;
	GetWorld()->OverlapMultiByChannel(OutResults, Instigator->GetActorLocation(), FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Instigator->GetCombatComponent()->CombatAcceptanceRadius), Params);
	DrawDebugSphere(GetWorld(), Instigator->GetActorLocation(), Instigator->GetCombatComponent()->CombatAcceptanceRadius, 30, FColor::Magenta, false, 5.0f);
	
	// find nearest
	AWandererBaseCharacter* NearestTarget = nullptr;
	float MinSquaredDist = 9999999.0f;
	for(FOverlapResult& Result : OutResults)
	{
		AWandererBaseCharacter* Target = Cast<AWandererBaseCharacter>(Result.GetActor());
		if(Target)
		{
			const float SquaredDist = FVector::DistSquared(Instigator->GetActorLocation(), Target->GetActorLocation());
			if(SquaredDist < MinSquaredDist)
			{
				NearestTarget = Target;
				MinSquaredDist = SquaredDist;
			}
		}
	}
	
	Instigator->GetCombatComponent()->SetCombatTarget(NearestTarget);
}