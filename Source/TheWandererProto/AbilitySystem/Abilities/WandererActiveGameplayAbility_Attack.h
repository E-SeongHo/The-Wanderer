//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_Attack.generated.h"

UENUM()
enum class EWandererAttackResult : uint8
{
	Success,
	Miss,
	Blocked,
};

class AWandererBaseCharacter;
class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS(Abstract)
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Attack : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWandererActiveGameplayAbility_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual void DetermineAttackAction() PURE_VIRTUAL(UWandererActiveGameplayAbility_Attack::DetermineAttackAction);

	virtual void ProcessAttack();
	
	virtual void SoftLock();
	
	void PlayNewMontageTask(UAnimMontage* MontageToPlay);

	UFUNCTION()
	void OnMontageCompleted();

	EWandererAttackResult EvaluateAttackResult(AWandererBaseCharacter* Target);
	
protected:
	TWeakObjectPtr<UAbilityTask_PlayMontageAndWait> CurrentPlayingMontageTask;
	FGameplayTag CurrentActionTag;
};
