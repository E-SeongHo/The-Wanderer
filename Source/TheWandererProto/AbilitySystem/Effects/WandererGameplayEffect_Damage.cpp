//  


#include "AbilitySystem/Effects/WandererGameplayEffect_Damage.h"

#include "WandererGameplayTags.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

UWandererGameplayEffect_Damage::UWandererGameplayEffect_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UWandererHealthAttributeSet::GetDamageAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat BaseDamage;
	BaseDamage.DataTag = WandererGameplayTags::Data_Damage_Base;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(BaseDamage);

	Modifiers.Add(ModifierInfo);

	FSetByCallerFloat CriticalDamage;
	CriticalDamage.DataTag = WandererGameplayTags::Data_Damage_Critical;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(CriticalDamage);

	Modifiers.Add(ModifierInfo);
}
