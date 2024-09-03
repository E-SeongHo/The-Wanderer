//  


#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "WandererGameplayTags.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/WandererCombatComponent.h"

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
	DrawDebugCircle(GetWorld(), Data.EffectSpec.GetEffectContext().GetHitResult()->Location, 10.0f, 30, FColor::Blue, true);

	SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Get Damaged %f, Current Health : %f"), GetDamage(), GetHealth()));
	Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_Hit));
}
