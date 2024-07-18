//  


#include "AbilitySystem/Abilities/Tasks/WandererAbilityTask_RepeatUntil.h"

void UWandererAbilityTask_RepeatUntil::PerformAction()
{
	OnPerformAction.Broadcast();
}

UWandererAbilityTask_RepeatUntil* UWandererAbilityTask_RepeatUntil::RepeatAction(UGameplayAbility* OwningAbility, float TimeBetweenActions)
{
	UWandererAbilityTask_RepeatUntil* MyObj = NewAbilityTask<UWandererAbilityTask_RepeatUntil>(OwningAbility);
	
	MyObj->TimeBetweenActions = TimeBetweenActions;

	return MyObj;
}

void UWandererAbilityTask_RepeatUntil::Activate()
{
	Super::Activate();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWandererAbilityTask_RepeatUntil::PerformAction, TimeBetweenActions, true);
}

void UWandererAbilityTask_RepeatUntil::OnDestroy(bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	Super::OnDestroy(bInOwnerFinished);
}

