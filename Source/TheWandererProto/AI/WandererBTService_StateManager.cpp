//  


#include "WandererBTService_StateManager.h"

#include "AbilitySystemComponent.h"
#include "WandererAIController.h"
#include "WandererGameplayTags.h"
#include "Character/Enemy/WandererEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/WandererCombatComponent.h"

UWandererBTService_StateManager::UWandererBTService_StateManager()
{
	NodeName = TEXT("AI State Manager");
	RandomDeviation = 1.0f;
}

void UWandererBTService_StateManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AWandererAIController* AIController = Cast<AWandererAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	AWandererEnemy* ControllingEnemy = AIController->GetControllingEnemy();
	UAbilitySystemComponent* ASC = ControllingEnemy->GetAbilitySystemComponent();
	UWandererCombatComponent* CombatComponent = ControllingEnemy->GetCombatComponent();
	
	AWandererBaseCharacter* SightTarget = Cast<AWandererBaseCharacter>(Blackboard->GetValue<UBlackboardKeyType_Object>(AIController->BBSightTargetKey));
	EWandererAIBehavior Behavior = EWandererAIBehavior::Idle;
	if(ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		if(SightTarget)
		{
			check(CombatComponent->GetCombatTarget() && CombatComponent->GetCombatTarget() == SightTarget);
			Blackboard->SetValue<UBlackboardKeyType_Vector>(AIController->BBCombatTargetLocationKey, CombatComponent->GetCombatTarget()->GetActorLocation());
		}
		else
		{
			CombatComponent->SetCombatTarget(nullptr);
		}

		if(CombatComponent->IsTargetInAttackRange()) // this also ensure target is set 
		{
			// maybe avoid or parry 
			Behavior = EWandererAIBehavior::Attack;
		}
		else
		{
			Behavior = EWandererAIBehavior::Approach;
		}
	}
	else
	{
		if(SightTarget)
		{
			check(!CombatComponent->GetCombatTarget());

			if(SightTarget->GetDistanceTo(ControllingEnemy) <= CombatComponent->CombatAcceptanceRadius)
			{
				// GA_DrawWeapon activated itself by setting combat target
				CombatComponent->SetCombatTarget(SightTarget);

				if(!SightTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
				{
					SightTarget->GetCombatComponent()->StartCombat();
				}
				return;
			}
		}
		
		Behavior = EWandererAIBehavior::Patrol;
	}

	Blackboard->SetValue<UBlackboardKeyType_Enum>(AIController->BBBehaviorTypeKey, static_cast<uint8>(Behavior));
}

