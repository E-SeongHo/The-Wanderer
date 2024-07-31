//  


#include "WandererBTDecorator_HasTag.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Character/Enemy/WandererEnemy.h"

bool UWandererBTDecorator_HasTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AWandererEnemy* ControllingEnemy = Cast<AWandererEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	check(ControllingEnemy);
	check(ControllingEnemy->GetAbilitySystemComponent());

	return ControllingEnemy->GetAbilitySystemComponent()->HasMatchingGameplayTag(GameplayTag);
}
