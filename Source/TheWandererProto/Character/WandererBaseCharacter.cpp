 // Fill out your copyright notice in the Description page of Project Settings.


#include "WandererBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "WandererCombatComponent.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"

AWandererBaseCharacter::AWandererBaseCharacter()
{
	CombatComponent = CreateDefaultSubobject<UWandererCombatComponent>(TEXT("Combat Component"));
}

void AWandererBaseCharacter::Die()
{
	GetCharacterMovement()->DisableMovement();
}

void AWandererBaseCharacter::GrantStartupAbilities()
{
	// Grant Startup Active Abilities
	// I'm considering it might be better if it happens in ASC class?
	for(auto ActiveAbility : StartupActiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ActiveAbility, 1);

		if(const UWandererActiveGameplayAbility* WandererActiveGA = Cast<UWandererActiveGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(WandererActiveGA->InputTag);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	// Grant Startup Triggered Abilities ( until now, it just means all abilities which doesn't need an input )
	for(auto TriggeredAbility : StartupTriggeredAbilities)
	{
 		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(TriggeredAbility, 1);

 		if(const UWandererGameplayAbility* WandererGA = Cast<UWandererGameplayAbility>(AbilitySpec.Ability))
 		{
 			AbilitySystemComponent->GiveAbility(AbilitySpec);
 		}
	}
}



