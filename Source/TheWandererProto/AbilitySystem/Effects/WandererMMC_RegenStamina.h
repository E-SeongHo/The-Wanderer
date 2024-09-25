//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "WandererMMC_RegenStamina.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererMMC_RegenStamina : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UWandererMMC_RegenStamina();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition ResilienceDef;
};
