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
	
	virtual bool CanRetrigger() const { return bRetriggerInstancedAbility; }
	
protected:
	void AddLooseTagToOwner(const FGameplayTag& GameplayTag) const;
	void RemoveLooseTagFromOwner(const FGameplayTag& GameplayTag) const;
	bool DoesOwnerHaveTag(const FGameplayTag& GameplayTag) const;
	
	UAnimMontage* GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const;

	UWandererMontagePair* GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const;
};
