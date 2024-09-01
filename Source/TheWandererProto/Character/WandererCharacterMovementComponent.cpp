//  


#include "WandererCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "WandererGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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

	if(Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Parry))
	{
		return MaxWalkSpeed * 0.5f;
	}
	
	if(Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		return MaxWalkSpeed * 0.65f;
	}
	
	switch(UprightLocomotionMode)
	{
	case EWandererUprightMovement::Run:
		return MaxWalkSpeed;
	case EWandererUprightMovement::Walk:
		return MaxWalkSpeed * 0.5f;
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

void UWandererCharacterMovementComponent::OnFoleyMovementEvent()
{
	if(MovementMode != MOVE_Walking) return;

	float Multiplier = 1.0f;
	if(IsCrouching())
	{
		Multiplier = 0.3f;
	}
	else
	{
		switch(UprightLocomotionMode)
		{
		case EWandererUprightMovement::Walk:
			Multiplier = 0.7f;
			break;
		case EWandererUprightMovement::Run:
			Multiplier = 1.0f;
			break;
		case EWandererUprightMovement::Sprint:
			Multiplier = 1.5f;
			break;
		}
	}

	UGameplayStatics::PlaySoundAtLocation(this, FootStepSoundCue, GetOwner()->GetActorLocation(), Multiplier, Multiplier);

	APawn* Owner = GetPawnOwner();
	if(Owner->IsPlayerControlled() && !IsCrouching())
	{
		Owner->MakeNoise(Multiplier, Owner, Owner->GetActorLocation(), 1000.0f);
	}
}

void UWandererCharacterMovementComponent::SetUprightLocomotionMode(const EWandererUprightMovement InMode)
{
	//check(InMode != UprightLocomotionMode);
	UprightLocomotionMode = InMode;
}


