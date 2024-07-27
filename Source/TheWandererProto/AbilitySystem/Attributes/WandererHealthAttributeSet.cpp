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
	
	AActor* Instigator = CastChecked<IAbilitySystemInterface>(Data.EffectSpec.GetEffectContext().GetOriginalInstigator())->GetAbilitySystemComponent()->GetAvatarActor();
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (GetDamage() > 0.0f)
		{
			// Parry
			if(Data.Target.HasMatchingGameplayTag(WandererGameplayTags::State_Combat_Parry))
			{
				FGameplayEventData EventData;
				EventData.Instigator = Instigator;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), WandererGameplayTags::Event_Combat_ParryAttack, EventData);
			}
			// Hit
			else
			{
				SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Get Damaged %f, Current Health : %f"), GetDamage(), GetHealth()));

				// transfer to combat mode if it was not in combat
				AWandererBaseCharacter* WandererBaseInstigator = Cast<AWandererBaseCharacter>(Instigator);
				if(WandererBaseInstigator)
				{
					if(!WandererBaseInstigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
					{
						WandererBaseInstigator->GetCombatComponent()->StartCombat();
					}
				}
				AWandererBaseCharacter* WandererBaseTarget = Cast<AWandererBaseCharacter>(Data.Target.GetAvatarActor());
				if(WandererBaseTarget)
				{
					if(!WandererBaseTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
					{
						WandererBaseTarget->GetCombatComponent()->StartCombat();
					}
				}
				
				Data.Target.TryActivateAbilitiesByTag(FGameplayTagContainer(WandererGameplayTags::Ability_Hit));
			}
		}
		
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
