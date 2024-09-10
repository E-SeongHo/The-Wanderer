//  


#include "WandererBTTask_StartCombat.h"

#include "AbilitySystemComponent.h"
#include "WandererAIController.h"
#include "WandererGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Character/Component/WandererCombatComponent.h"
#include "Character/Enemy/WandererEnemy.h"

UWandererBTTask_StartCombat::UWandererBTTask_StartCombat()
{
	NodeName = TEXT("AI Start Combat");
}

EBTNodeResult::Type UWandererBTTask_StartCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWandererAIController* AIController = Cast<AWandererAIController>(OwnerComp.GetAIOwner());
	AWandererEnemy* ControllingEnemy = AIController->GetControllingEnemy();

	check(!ControllingEnemy->GetCombatComponent()->GetCombatTarget());
	
	AWandererBaseCharacter* SightTarget = Cast<AWandererBaseCharacter>(AIController->GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(AIController->BBSightTargetKey));
	ControllingEnemy->GetCombatComponent()->SetCombatTarget(SightTarget);

	if(!SightTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		SightTarget->GetCombatComponent()->StartCombat();
	}

	return EBTNodeResult::Succeeded;
}
