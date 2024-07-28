//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_TargetLock.generated.h"

class AWandererCharacter;
class AWandererEnemy;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_TargetLock : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_TargetLock();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	AWandererEnemy* FindTarget(const AWandererCharacter* SrcCharacter);

	UFUNCTION()
	void OnTargetLost();

	UFUNCTION()
	void MonitorTarget();

private:
	TWeakObjectPtr<AWandererEnemy> Target;
};
