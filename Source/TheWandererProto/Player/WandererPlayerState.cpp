//  


#include "Player/WandererPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"

AWandererPlayerState::AWandererPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	HealthAttributeSet = CreateDefaultSubobject<UWandererHealthAttributeSet>("Health AttributeSet");
	CombatAttributeSet = CreateDefaultSubobject<UWandererCombatAttributeSet>("Combat AttributeSet");
	
	// in case of multi-player game
	//AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	//NetUpdateFrequency = 100.f;
}

