//  


#include "AI/WandererBTDecorator_CanAttack.h"

#include "AIController.h"
#include "Character/Component/WandererCombatComponent.h"
#include "Character/Enemy/WandererEnemy.h"

bool UWandererBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	check(OwnerComp.GetAIOwner());
	
	AWandererEnemy* ControllingEnemy = Cast<AWandererEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	
	check(ControllingEnemy);

	return ControllingEnemy->GetCombatComponent()->IsTargetInAttackRange();
}
