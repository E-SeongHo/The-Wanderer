//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Tasks/WandererAbilityTask.h"
#include "WandererAbilityTask_RepeatUntil.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRepeatedAction);

/**
 *  This class is designed because AbilityTask_Repeat doesn't support unlimited repetition
 *  Almost everything is the same, except it runs infinitely
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAbilityTask_RepeatUntil : public UWandererAbilityTask
{
	GENERATED_BODY()

public:
	FRepeatedAction OnPerformAction;
	
	void PerformAction();
	
	static UWandererAbilityTask_RepeatUntil* RepeatAction(UGameplayAbility* OwningAbility, float TimeBetweenActions);

	virtual void Activate() override;
	
private:
	float TimeBetweenActions;

	FTimerHandle TimerHandle;

	virtual void OnDestroy(bool bInOwnerFinished) override;
};
