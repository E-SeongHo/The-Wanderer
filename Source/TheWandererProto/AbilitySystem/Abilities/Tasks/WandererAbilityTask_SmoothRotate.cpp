//  


#include "AbilitySystem/Abilities/Tasks/WandererAbilityTask_SmoothRotate.h"

#include "GameFramework/Character.h"

UWandererAbilityTask_SmoothRotate* UWandererAbilityTask_SmoothRotate::SmoothRotate(UGameplayAbility* OwningAbility, const FRotator StartRotation, const FRotator GoalRotation)
{
	UWandererAbilityTask_SmoothRotate* MyObj = NewAbilityTask<UWandererAbilityTask_SmoothRotate>(OwningAbility);

	MyObj->ActorToRotate = OwningAbility->GetCurrentActorInfo()->AvatarActor;
	MyObj->CurrentRotation = StartRotation;
	MyObj->GoalRotation = GoalRotation;

	return MyObj;
}

void UWandererAbilityTask_SmoothRotate::Activate()
{
	if((CurrentRotation.Vector() - GoalRotation.Vector()).Size() < RotationThreshold)
	{
		GetWorld()->GetTimerManager().SetTimer(RotateTimerHandle, this, &UWandererAbilityTask_SmoothRotate::RotateTick, 0.1f, true, false);
	}
	else
	{
		EndTask();
	}
}

void UWandererAbilityTask_SmoothRotate::OnDestroy(bool bInOwnerFinished)
{
	GetWorld()->GetTimerManager().ClearTimer(RotateTimerHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UWandererAbilityTask_SmoothRotate::RotateTick()
{
	ACharacter* Character = Cast<ACharacter>(ActorToRotate);
	DrawDebugLine(ActorToRotate->GetWorld(), ActorToRotate->GetActorLocation(), ActorToRotate->GetActorLocation() + ActorToRotate->GetActorForwardVector() * 100.0f, FColor::Red, false, 1.0f);
	if((CurrentRotation.Vector() - Character->GetControlRotation().Vector()).Size() < RotationThreshold)
	{
		CurrentRotation = FMath::RInterpTo(CurrentRotation, Character->GetControlRotation(), GetWorld()->GetDeltaSeconds(), 10.0f);
		ActorToRotate->SetActorRotation(CurrentRotation);
	}
	else
	{
		EndTask();
	}
}
