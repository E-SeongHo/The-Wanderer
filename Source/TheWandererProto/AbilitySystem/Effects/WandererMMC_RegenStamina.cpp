//  


#include "AbilitySystem/Effects/WandererMMC_RegenStamina.h"

#include "WandererGameplayTags.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

UWandererMMC_RegenStamina::UWandererMMC_RegenStamina()
{
	ResilienceDef.AttributeToCapture = UWandererHealthAttributeSet::GetResilienceAttribute();
	ResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResilienceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ResilienceDef);
}

float UWandererMMC_RegenStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	if(TargetTags->HasTag(WandererGameplayTags::State_Dead)) return 0.0f;
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Resilience = 0.0f;
	GetCapturedAttributeMagnitude(ResilienceDef, Spec, EvaluationParameters, Resilience);
	Resilience = FMath::Max<float>(Resilience, 0.0f);

	return Resilience * 0.01f;
}
