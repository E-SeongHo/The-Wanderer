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
	UFUNCTION()
	static UWandererAbilityTask_SmoothRotate* SmoothRotate(UGameplayAbility* OwningAbility, const FRotator StartRotation, const FRotator GoalRotation);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;
	
private:
	void RotateTick();
	
private:
	TWeakObjectPtr<AActor> ActorToRotate;
	FRotator CurrentRotation;
	FRotator GoalRotation;

	FTimerHandle RotateTimerHandle;

	float RotationThreshold = 5.0f;
};
