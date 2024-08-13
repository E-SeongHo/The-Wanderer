//  

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WandererGameplayAbility.generated.h"

class UWandererMontagePair;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class THEWANDERERPROTO_API UWandererGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWandererGameplayAbility();
	
	bool CanRetrigger() const { return bRetriggerInstancedAbility; }
	
protected:
	UAnimMontage* GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const;

	UWandererMontagePair* GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const;
};
