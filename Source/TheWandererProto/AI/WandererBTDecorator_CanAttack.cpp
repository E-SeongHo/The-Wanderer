//  


#include "AI/WandererBTDecorator_CanAttack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility_Melee.h"
#include "Character/Component/WandererCombatComponent.h"
#include "Character/Enemy/WandererEnemy.h"

bool UWandererBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	check(OwnerComp.GetAIOwner());
	
	const AWandererEnemy* ControllingEnemy = Cast<AWandererEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	check(ControllingEnemy);
	
	if(ControllingEnemy->GetCombatComponent()->IsTargetInAttackRange())
	{
		return true;
	}
	
	return true;
}
