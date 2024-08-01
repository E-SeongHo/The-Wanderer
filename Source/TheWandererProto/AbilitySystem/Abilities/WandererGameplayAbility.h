//  

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WandererGameplayAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class THEWANDERERPROTO_API UWandererGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UAnimMontage* GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const;
};
