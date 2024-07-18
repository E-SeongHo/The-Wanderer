//  


#include "Character/WandererCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "WandererEnemy.h"
#include "WandererGameplayTags.h"
#include "Camera/CameraComponent.h"

UWandererCombatComponent::UWandererCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWandererCombatComponent::StartCombat()
{
	AbilitySystemComponent->AddLooseGameplayTag(WandererGameplayTags::State_Combat);
	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_AutoTarget));
	// TODO: Zoom-out camera : i want to see the combat in wide-scene 
}

void UWandererCombatComponent::LockOnTarget(AWandererBaseCharacter* TargetToLock)
{
	if(TargetToLock)
	{
		CastChecked<AWandererEnemy>(TargetToLock)->SetUIRender(true);
	}
	if(LockTarget)
	{
		CastChecked<AWandererEnemy>(LockTarget)->SetUIRender(false);
	}
	LockTarget = TargetToLock;
}

void UWandererCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(AbilitySystemComponent == nullptr) return;
	
	if(IsLockingOn())
	{
		// TODO: maybe define corresponding camera mode?
		
		check(LockTarget);
		AWandererBaseCharacter* Owner = Cast<AWandererBaseCharacter>(GetOwner());
		if(AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat_TargetLock))
		{
			const float Distance = FVector::Distance(Owner->GetActorLocation(), LockTarget->GetActorLocation());
			if(Distance > LockOnDistance)
			{
				OnTargetLost.Broadcast();
			}
			else
			{
				const FVector ToTarget = LockTarget->GetActorLocation() - Owner->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
				Owner->GetController()->SetControlRotation(ToTarget.Rotation()); // lagged camera rotation makes this rotate smoothly
			}
		}
		else if(AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
		{
			const FVector Forward = Owner->GetActorForwardVector();
			const FVector ToTarget = (LockTarget->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal2D();
			Owner->SetActorRotation(FMath::RInterpTo(Forward.Rotation(), ToTarget.Rotation(), DeltaTime, 5.0f));
		}
	}
}

bool UWandererCombatComponent::IsLockingOn() const
{
	return LockTarget != nullptr;
}



