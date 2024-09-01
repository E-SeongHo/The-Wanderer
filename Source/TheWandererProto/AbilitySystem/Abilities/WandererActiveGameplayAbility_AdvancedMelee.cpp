//  


#include "AbilitySystem/Abilities/WandererActiveGameplayAbility_AdvancedMelee.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Utility/WandererUtils.h"
#include "Animation/WandererAnimMontageConfig.h"

UWandererActiveGameplayAbility_AdvancedMelee::UWandererActiveGameplayAbility_AdvancedMelee()
{
}

void UWandererActiveGameplayAbility_AdvancedMelee::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	// before super's PreActivate() cancels ability_parry, check can counter
	const AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());
	const AWandererBaseCharacter* CombatTarget = Instigator->GetCombatComponent()->GetCombatTarget(); 

	const bool bIsParrying = DoesOwnerHaveTag(WandererGameplayTags::State_Parry);
	const bool bIsTargetAttacking = CombatTarget && CombatTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::Ability_Attack);  

	bCanCounter = bIsParrying && bIsTargetAttacking;
	
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

void UWandererActiveGameplayAbility_AdvancedMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());

	// check can set target nearby, especially it specify about to attack from behind without detected
	if(!DoesOwnerHaveTag(WandererGameplayTags::State_Combat))
	{
		AWandererBaseCharacter* ForwardTarget = FindNearestOverlapTargetInDirection(Instigator->GetActorForwardVector(), 120.0f, 150.0f);
		if(ForwardTarget)
		{
			Instigator->GetCombatComponent()->SetCombatTarget(ForwardTarget);
		}
	}

	if(Instigator->GetCombatComponent()->CanFinishTarget())
	{
		TriggerFinisher();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UWandererActiveGameplayAbility_AdvancedMelee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_AdvancedMelee::DetermineAttackAction()
{
	const AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());

	const FVector InputDirection = Instigator->GetLastMovementInputVector();
	const EDirection EvaluatedInputDirection = WandererUtils::EvaluateDirectionRelativeToActor(Instigator, InputDirection);
	bCanCounter = bCanCounter && (EvaluatedInputDirection == EDirection::Right || EvaluatedInputDirection == EDirection::Left);
	if(bCanCounter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Counter")));

		if(EvaluatedInputDirection == EDirection::Right)
		{
			CurrentActionTag= WandererGameplayTags::ActionTag_Attack_Counter_Right;
		}
		else
		{
			CurrentActionTag= WandererGameplayTags::ActionTag_Attack_Counter_Left;
		}
		return;
	}

	if(!DoesOwnerHaveTag(WandererGameplayTags::State_Combat_TargetLock))
	{
		// Check if the target can be changed to the desired movement input direction
		AWandererBaseCharacter* InputDirectionTarget = FindNearestOverlapTargetInDirection(InputDirection, 120.0f, Instigator->GetCombatComponent()->CombatAcceptanceRadius);
		if(Instigator->GetCombatComponent()->CanDashTo(InputDirectionTarget))
		{
			Instigator->GetCombatComponent()->SetCombatTarget(InputDirectionTarget);
		}
	}
	
	if(!Instigator->GetCombatComponent()->IsTargetInAttackRange() && Instigator->GetCombatComponent()->IsTargetInDashRange())
	{
		CurrentActionTag= WandererGameplayTags::ActionTag_Attack_Dash;
		return;
	}
	
	Super::DetermineAttackAction();
}

void UWandererActiveGameplayAbility_AdvancedMelee::SoftLock()
{
	const AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());
	const AWandererBaseCharacter* CombatTarget = Instigator->GetCombatComponent()->GetCombatTarget();

	Instigator->GetMotionWarpComponent()->RemoveWarpTarget(TEXT("SideStepTarget"));
	Instigator->GetMotionWarpComponent()->RemoveWarpTarget(TEXT("AttackTarget"));

	if(bCanCounter)
	{
		FVector SideStepLocation;
		if(CurrentActionTag == WandererGameplayTags::ActionTag_Attack_Counter_Left)
		{
			const FVector CombatTargetForwardRight = (CombatTarget->GetActorForwardVector() + CombatTarget->GetActorRightVector()).GetSafeNormal();
			SideStepLocation = CombatTarget->GetActorLocation() + CombatTargetForwardRight * 100.0f;
		}
		else
		{
			const FVector CombatTargetForwardLeft = (CombatTarget->GetActorForwardVector() + -CombatTarget->GetActorRightVector()).GetSafeNormal();
			SideStepLocation = CombatTarget->GetActorLocation() + CombatTargetForwardLeft * 100.0f;
		}

		Instigator->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("SideStepTarget"), SideStepLocation, (-CombatTarget->GetActorForwardVector()).Rotation());
	}

	if(Instigator->GetCombatComponent()->IsTargetInDashRange() && CurrentActionTag == WandererGameplayTags::ActionTag_Attack_Dash)
	{
		const FVector WarpDirection = (CombatTarget->GetActorLocation() - Instigator->GetActorLocation()).GetSafeNormal2D();
		const FVector WarpLocation = Instigator->GetDistanceTo(CombatTarget) > 150.0f ? CombatTarget->GetActorLocation() - WarpDirection * 150.0f : Instigator->GetActorLocation();

		DrawDebugCircle(GetWorld(), WarpLocation, 30.0f, 10, FColor::Red, false, 3.0f);
		Instigator->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackTarget"), WarpLocation, WarpDirection.Rotation());
	}
	else
	{
		Super::SoftLock();
	}
}

void UWandererActiveGameplayAbility_AdvancedMelee::TriggerFinisher() const
{
	AWandererCharacter* Instigator = CastChecked<AWandererCharacter>(GetAvatarActorFromActorInfo());
	AWandererBaseCharacter* CombatTarget = Instigator->GetCombatComponent()->GetCombatTarget();
	
	FGameplayTag InstigatorActionTag;
	if(WandererUtils::EvaluateDirectionRelativeToActor(Instigator, CombatTarget) == EDirection::Backward)
	{
		InstigatorActionTag = WandererGameplayTags::ActionTag_Pair_Finisher_Back;
	}
	else
	{
		InstigatorActionTag = WandererGameplayTags::ActionTag_Pair_Finisher_Front;
	}
	
	FGameplayEventData EventData;
	EventData.OptionalObject = GetMatchingMontagePairForTag(InstigatorActionTag);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, WandererGameplayTags::Event_Combat_Finisher, EventData);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(CombatTarget, WandererGameplayTags::Event_Combat_Victim, EventData);
}

AWandererBaseCharacter* UWandererActiveGameplayAbility_AdvancedMelee::FindNearestOverlapTargetInDirection(const FVector& Direction, const float Angle, const float Distance) const
{
	const AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());

	TArray<AActor*> OverlapTargets = WandererUtils::FindOverlappingActorsInViewRange(AWandererBaseCharacter::StaticClass(), Instigator, Direction, Angle, Distance, ECC_GameTraceChannel1);
	AWandererBaseCharacter* NearestTarget = nullptr;
	float MinDist = 9999.0f;

	for(AActor* TargetActor : OverlapTargets)
	{
		AWandererBaseCharacter* Target = Cast<AWandererBaseCharacter>(TargetActor);
		if(!Target) continue;

		const float Dist = Instigator->GetDistanceTo(Target);
		if(Dist < MinDist)
		{
			MinDist = Dist;
			NearestTarget = Target;
		}
	}

	return NearestTarget;
}
