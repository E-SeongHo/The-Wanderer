//  


#include "AbilitySystem/Abilities/Tasks/WandererAbilityTask_SmoothRotate.h"

UWandererAbilityTask_SmoothRotate* UWandererAbilityTask_SmoothRotate::SmoothRotate(UGameplayAbility* OwningAbility, const FRotator StartRotation, const FRotator GoalRotation)
{
	UWandererAbilityTask_SmoothRotate* MyObj = NewAbilityTask<UWandererAbilityTask_SmoothRotate>(OwningAbility);

	MyObj->ActorToRotate = OwningAbility->GetCurrentActorInfo()->AvatarActor;
	MyObj->CurrentRotation = StartRotation;
	MyObj->GoalRotation = FRotator(StartRotation.Pitch, GoalRotation.Yaw, StartRotation.Roll);

	return MyObj;
}

void UWandererAbilityTask_SmoothRotate::Activate()
{
	if(!CurrentRotation.Equals(GoalRotation, RotationThreshold))
	{
		bTickingTask = true;
	}
	else
	{
		EndTask();
	}
}

void UWandererAbilityTask_SmoothRotate::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UWandererAbilityTask_SmoothRotate::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if(!CurrentRotation.Equals(GoalRotation, RotationThreshold))
	{
		CurrentRotation = FMath::RInterpTo(CurrentRotation, GoalRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
		ActorToRotate->SetActorRotation(CurrentRotation);
	}
	else
	{
		EndTask();
	}
}