//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Tasks/WandererAbilityTask.h"
#include "WandererAbilityTask_SmoothRotate.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAbilityTask_SmoothRotate : public UWandererAbilityTask
{
	GENERATED_BODY()

public:
	// It only adjusts Yaw
	UFUNCTION()
	static UWandererAbilityTask_SmoothRotate* SmoothRotate(UGameplayAbility* OwningAbility, const FRotator StartRotation, const FRotator GoalRotation);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	virtual void TickTask(float DeltaTime) override;
	
private:
	TWeakObjectPtr<AActor> ActorToRotate;
	FRotator CurrentRotation;
	FRotator GoalRotation;

	float RotationThreshold = 5.0f;
};
