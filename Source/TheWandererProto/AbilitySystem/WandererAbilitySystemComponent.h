//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WandererAbilitySystemComponent.generated.h"

/**
 * currently nothing
 * if held ability needed, it will be expanded
 * e.g) ability_fly ? charge ? 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;

	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void AbilitySpecInputHeld(FGameplayAbilitySpec& Spec);
};
