//  


#include "Character/WandererCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "WandererGameplayTags.h"
#include "Camera/CameraComponent.h"

UWandererCombatComponent::UWandererCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWandererCombatComponent::LockOnTarget(AWandererBaseCharacter* TargetToLock)
{
	LockTarget = TargetToLock;	
}

void UWandererCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsLockingOn())
	{
		check(LockTarget);

		AWandererBaseCharacter* Owner = Cast<AWandererBaseCharacter>(GetOwner());
		const float Distance = FVector::Distance(Owner->GetActorLocation(), LockTarget->GetActorLocation());
		if(Distance > LockOnDistance)
		{
			OnTargetLost.Broadcast();
		}
		else
		{
			const FVector ToTarget = LockTarget->GetActorLocation() - Owner->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
			Owner->GetController()->SetControlRotation(ToTarget.Rotation());
		}
	}
}

bool UWandererCombatComponent::IsLockingOn() const
{
	return AbilitySystemComponent != nullptr && AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat_TargetLock);
}



