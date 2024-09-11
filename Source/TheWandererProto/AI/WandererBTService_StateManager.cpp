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
#include "Character/Component/WandererCombatComponent.h"

UWandererBTService_StateManager::UWandererBTService_StateManager()
{
	NodeName = TEXT("AI State Manager");
	RandomDeviation = 1.0f;
}

void UWandererBTService_StateManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("State AI")));

	AWandererAIController* AIController = Cast<AWandererAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	AWandererEnemy* ControllingEnemy = AIController->GetControllingEnemy();
	UAbilitySystemComponent* ASC = ControllingEnemy->GetAbilitySystemComponent();
	UWandererCombatComponent* CombatComponent = ControllingEnemy->GetCombatComponent();
	
	AWandererBaseCharacter* SightTarget = Cast<AWandererBaseCharacter>(Blackboard->GetValue<UBlackboardKeyType_Object>(AIController->BBSightTargetKey));
	if(SightTarget)
	{
		if(SightTarget->GetDistanceTo(ControllingEnemy) <= CombatComponent->CombatAcceptanceRadius)
		{
			// GA_DrawWeapon activated by setting combat target if it wasn't in combat
			CombatComponent->SetCombatTarget(SightTarget);

			if(!SightTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
			{
				SightTarget->GetCombatComponent()->StartCombat();
			}
		}
	}
	else
	{
		CombatComponent->SetCombatTarget(nullptr);
	}
	
	EWandererAIBehavior Behavior = EWandererAIBehavior::Idle;
	if(ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		if(CombatComponent->GetCombatTarget())
		{
			check(CombatComponent->GetCombatTarget() == SightTarget);
			Blackboard->SetValue<UBlackboardKeyType_Vector>(AIController->BBCombatTargetLocationKey, CombatComponent->GetCombatTarget()->GetActorLocation());
		}
		
		if(CombatComponent->IsTargetInAttackRange())
		{
			// TODO: maybe avoid or parry
			Behavior = EWandererAIBehavior::Attack;
		}
		else
		{
			// if get lost combat target, and combat exit delaying is still on activate, approach to last seen location
			// considering expand logic to recon somewhere?
			Behavior = EWandererAIBehavior::Approach;
		}
	}
	else
	{
		Behavior = EWandererAIBehavior::Patrol;
	}	

	Blackboard->SetValue<UBlackboardKeyType_Enum>(AIController->BBBehaviorTypeKey, static_cast<uint8>(Behavior));
}

