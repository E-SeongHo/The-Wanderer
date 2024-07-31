//  


#include "WandererCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "WandererGameplayTags.h"

UWandererCharacterMovementComponent::UWandererCharacterMovementComponent()
	: UprightLocomotionMode(DefaultUprightLocomotion)
{
}

float UWandererCharacterMovementComponent::GetMaxSpeed() const
{
	if(MovementMode != MOVE_Walking) return Super::GetMaxSpeed();
	if(IsCrouching()) return MaxWalkSpeedCrouched;

	AWandererBaseCharacter* Owner = Cast<AWandererBaseCharacter>(GetOwner());
	if(!Owner) return Super::GetMaxSpeed();

	if(Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		return MaxWalkSpeed * 0.5f;
	}
	if(Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Parry))
	{
		return MaxWalkSpeed * 0.5f;
	}
	
	switch(UprightLocomotionMode)
	{
	case EWandererUprightMovement::Run:
		return MaxWalkSpeed;
	case EWandererUprightMovement::Walk:
		return MaxWalkSpeed * 0.4f;
	case EWandererUprightMovement::Sprint:
		return MaxWalkSpeed * 1.5f;
	}

	return Super::GetMaxSpeed();
}

void UWandererCharacterMovementComponent::StartSprinting()
{
	SetUprightLocomotionMode(EWandererUprightMovement::Sprint); 
}

void UWandererCharacterMovementComponent::StopSprinting()
{
	SetUprightLocomotionMode(DefaultUprightLocomotion); 
}

void UWandererCharacterMovementComponent::StartWalking()
{
	SetUprightLocomotionMode(EWandererUprightMovement::Walk); 
}

void UWandererCharacterMovementComponent::StopWalking()
{
	SetUprightLocomotionMode(DefaultUprightLocomotion); 
}

void UWandererCharacterMovementComponent::SetUprightLocomotionMode(const EWandererUprightMovement InMode)
{
	//check(InMode != UprightLocomotionMode);
	UprightLocomotionMode = InMode;
}


