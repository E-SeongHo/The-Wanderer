// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "WandererAnimMontageConfig.h"
#include "WandererGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TheWandererProto/Character/WandererCharacter.h"

// This function 
// 1. When Derived ABP Created
// 2. When ABP Activated in Game 
void UWandererAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UWandererAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AWandererBaseCharacter* Character = Cast<AWandererBaseCharacter>(GetOwningActor());
	if (!Character) return;
	const UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!ASC) return;
	
	MovementComp = Character->GetCharacterMovement();
		
	Velocity = MovementComp->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed >= 3.0f && MovementComp->GetCurrentAcceleration().Length();
	bIsFalling = MovementComp->IsFalling();
	bIsCrouched = MovementComp->IsCrouching();
	bIsDefensing = ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Parry);
	bIsInCombat = ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat);
	
	TrajectoryComp = Character->GetTrajectoryComponent();
}

UAnimMontage* UWandererAnimInstance::GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const
{
	check(MontageConfig);
	// check(MontageConfig->HasExactMatchingActionMontage(GameplayTag));	
	return MontageConfig->FindAnimMontageForTag(GameplayTag);
}

UWandererMontagePair* UWandererAnimInstance::GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const
{
	check(MontageConfig);
	return MontageConfig->FindAnimMontagePairForTag(GameplayTag);
}
