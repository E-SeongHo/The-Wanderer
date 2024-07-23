// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererAnimInstance.h"

#include "AbilitySystemComponent.h"
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

	const AWandererCharacter* Character = Cast<AWandererCharacter>(GetOwningActor());
	if (!Character) return;

	MovementComp = Character->GetCharacterMovement();
		
	Velocity = MovementComp->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed >= 3.0f && MovementComp->GetCurrentAcceleration().Length();
	bIsFalling = MovementComp->IsFalling();
	bIsCrouched = MovementComp->IsCrouching();
	bIsDefensing = Character->GetAbilitySystemComponent() ? Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat_Parry) : false;
	
	TrajectoryComp = Character->GetTrajectoryComponent();
}
