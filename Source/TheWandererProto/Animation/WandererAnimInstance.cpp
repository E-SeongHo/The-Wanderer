// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "KismetAnimationLibrary.h"
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

	bIsCombatPSDSet = CombatPoseSearchDatabase != nullptr;
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
	GroundDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

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

TArray<UAnimMontage*> UWandererAnimInstance::GetMatchingComboMontageForTag(const FGameplayTag& GameplayTag, const FGameplayTag& SpecificTag) const
{
	check(MontageConfig);
	return MontageConfig->FindComboMontageForTag(GameplayTag, SpecificTag);	
}

UWandererMontagePair* UWandererAnimInstance::GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const
{
	check(MontageConfig);
	return MontageConfig->FindAnimMontagePairForTag(GameplayTag);
}
