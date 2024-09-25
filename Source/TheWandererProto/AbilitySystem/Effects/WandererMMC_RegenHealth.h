//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "WandererMMC_RegenHealth.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererMMC_RegenHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UWandererMMC_RegenHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition ResilienceDef;
	FGameplayEffectAttributeCaptureDefinition StaminaDef;
	FGameplayEffectAttributeCaptureDefinition MaxStaminaDef;
};
