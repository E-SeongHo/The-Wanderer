//  

#pragma once

#include "CoreMinimal.h"
#include "IRetriggerable.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "WandererGameplayAbility_KnockBack.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayAbility_KnockBack : public UWandererGameplayAbility, public IRetriggerable
{
	GENERATED_BODY()

public:
	UWandererGameplayAbility_KnockBack();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanRetrigger() const override;
	
private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnRecoveryAvailable(FGameplayEventData Payload);
};
