//  


#include "Character/WandererActorComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"

UWandererActorComponent::UWandererActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWandererActorComponent::AssignAbilitySystemComponent(UAbilitySystemComponent* OwnerASC)
{
	const AWandererBaseCharacter* Owner = Cast<AWandererBaseCharacter>(GetOwner());
	check(Owner);

	check(Owner->GetAbilitySystemComponent() == OwnerASC);

	AbilitySystemComponent = OwnerASC;
}


