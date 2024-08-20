//  


#include "AI/WandererBTTask_ActivateGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "WandererAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Character/Enemy/WandererEnemy.h"

UWandererBTTask_ActivateGameplayAbility::UWandererBTTask_ActivateGameplayAbility()
{
	NodeName = TEXT("Activate Gameplay Ability");
}

EBTNodeResult::Type UWandererBTTask_ActivateGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWandererAIController* AIController = CastChecked<AWandererAIController>(OwnerComp.GetAIOwner());
	UAbilitySystemComponent* ASC = AIController->GetControllingEnemy()->GetAbilitySystemComponent();

	const bool bSuccess = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
	if(!bSuccess)
	{
		AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(AIController->BBBehaviorTypeKey, static_cast<uint8>(EWandererAIBehavior::Wait));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
