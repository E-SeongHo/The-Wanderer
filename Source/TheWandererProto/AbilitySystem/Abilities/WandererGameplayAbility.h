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
	
protected:
	void AddLooseTagToOwner(const FGameplayTag& GameplayTag) const;
	void RemoveLooseTagFromOwner(const FGameplayTag& GameplayTag) const;
	bool DoesOwnerHaveTag(const FGameplayTag& GameplayTag) const;
	
	UAnimMontage* GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const;

	TArray<UAnimMontage*> GetMatchingComboMontageForTag(const FGameplayTag& GameplayTag, const FGameplayTag& SpecificTag = FGameplayTag()) const;

	UWandererMontagePair* GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const;
};
