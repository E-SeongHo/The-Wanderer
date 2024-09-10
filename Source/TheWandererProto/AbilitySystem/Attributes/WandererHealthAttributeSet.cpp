//  


#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "WandererGameplayTags.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/Component/WandererCombatComponent.h"

void UWandererHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UWandererHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		ProcessDamageEffect(Data);
		SetDamage(0.0f);
	}
	
	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		// TODO : DIE 
		bOutOfHealth = true;
		
		Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_Die));
		// bind delegate and broadcast
	}
}

void UWandererHealthAttributeSet::ProcessDamageEffect(const FGameplayEffectModCallbackData& Data)
{
	if (GetDamage() <= 0.0f) return;
	if (bOutOfHealth) return;
	
	AActor* Instigator = CastChecked<IAbilitySystemInterface>(Data.EffectSpec.GetEffectContext().GetOriginalInstigator())->GetAbilitySystemComponent()->GetAvatarActor();

	SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
	Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_Hit));
}
