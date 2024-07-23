//  


#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "WandererGameplayTags.h"

void UWandererHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UWandererHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (GetDamage() > 0.0f)
		{
			// Parry
			if(Data.Target.HasMatchingGameplayTag(WandererGameplayTags::State_Combat_Parry))
			{
				FGameplayEventData EventData;
				EventData.Instigator = Data.EffectSpec.GetEffectContext().GetEffectCauser();
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), WandererGameplayTags::Event_Combat_ParryAttack, EventData);
			}
			// Hit
			else
			{
				SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Get Damaged %f, Current Health : %f"), GetDamage(), GetHealth()));

				Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_Hit));
			}
		}
		
		SetDamage(0.0f);
	}

	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		// TODO : DIE 
		const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
		AActor* Instigator = EffectContext.GetOriginalInstigator();
		AActor* Causer = EffectContext.GetEffectCauser();
		
		bOutOfHealth = true;
		
		Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_Die));
		// bind delegate and broadcast
	}
}
