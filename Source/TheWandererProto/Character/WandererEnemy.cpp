//  


#include "Character/WandererEnemy.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"

AWandererEnemy::AWandererEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	HealthAttributeSet = CreateDefaultSubobject<UWandererHealthAttributeSet>("Health AttributeSet");
	CombatAttributeSet = CreateDefaultSubobject<UWandererCombatAttributeSet>("Combat AttributeSet");
}

void AWandererEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Init ASC
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	GrantStartupAbilities();
	
	// Default Gameplay Tag
	AbilitySystemComponent->AddLooseGameplayTag(WandererGameplayTags::State_Weapon_Unarmed);

	// Init Default Attributes By GE
	const UGameplayEffect* InitterGE = DefaultAttributesInitter->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(InitterGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
}
