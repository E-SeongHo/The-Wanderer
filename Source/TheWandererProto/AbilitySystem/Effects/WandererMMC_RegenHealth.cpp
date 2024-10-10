//  


#include "AbilitySystem/Effects/WandererMMC_RegenHealth.h"

#include "WandererGameplayTags.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

UWandererMMC_RegenHealth::UWandererMMC_RegenHealth()
{
	ResilienceDef.AttributeToCapture = UWandererHealthAttributeSet::GetResilienceAttribute();
	ResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResilienceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ResilienceDef);

	StaminaDef.AttributeToCapture = UWandererHealthAttributeSet::GetStaminaAttribute();
	StaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StaminaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StaminaDef);

	MaxStaminaDef.AttributeToCapture = UWandererHealthAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxStaminaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxStaminaDef);
}

float UWandererMMC_RegenHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	float Stamina = 0.0f;
	GetCapturedAttributeMagnitude(StaminaDef, Spec, EvaluationParameters, Stamina);
	Stamina = FMath::Max<float>(Stamina, 0.0f); 

	float MaxStamina = 0.0f;
	GetCapturedAttributeMagnitude(MaxStaminaDef, Spec, EvaluationParameters, MaxStamina);
	MaxStamina = FMath::Max<float>(MaxStamina, 0.0f); 

	// health can be regen if stamina fully charged
	return MaxStamina == Stamina ? Resilience * 0.01f : 0.0f;
}
