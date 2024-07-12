// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererAnimInstance.h"

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

	TrajectoryComp = Character->GetTrajectoryComponent();
}
