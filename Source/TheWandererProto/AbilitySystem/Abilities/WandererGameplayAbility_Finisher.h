//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "WandererGameplayAbility_Finisher.generated.h"

/**
 * This ability is only activated by a GA_Attack, during on combo attacking  
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayAbility_Finisher : public UWandererGameplayAbility
{
	GENERATED_BODY()

public:
	UWandererGameplayAbility_Finisher();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void CallEndAbility();
};