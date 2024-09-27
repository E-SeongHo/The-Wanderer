//  


#include "AI/WandererBTService_DetermineAction_Melee.h"

#include "AbilitySystemComponent.h"
#include "WandererAIController.h"
#include "WandererGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Component/WandererCombatComponent.h"
#include "Character/Enemy/WandererEnemy.h"
#include "Utility/WandererUtils.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

class UBlackboardKeyType_Enum;

UWandererBTService_DetermineAction_Melee::UWandererBTService_DetermineAction_Melee()
{
	NodeName = TEXT("Determine Melee Action");
	RandomDeviation = 1.0f;
}

void UWandererBTService_DetermineAction_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("State AI")));

	AWandererAIController* AIController = Cast<AWandererAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	AWandererEnemy* ControllingEnemy = AIController->GetControllingEnemy();
	UAbilitySystemComponent* ASC = ControllingEnemy->GetAbilitySystemComponent();
	UWandererCombatComponent* CombatComponent = ControllingEnemy->GetCombatComponent();
	
	// Since Enemy_Melee and Enemy_SwordShield share similar logic, the action is determined within the same task.
	// TODO: determining action can be affected by some Attributes
	EWandererAIAction Action = EWandererAIAction::Attack;
	if(CombatComponent->GetCombatTarget() && WandererUtils::EvaluateDirectionRelativeToActor(ControllingEnemy, CombatComponent->GetCombatTarget()) == EDirection::Forward)
	{
		if(ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Draw_Shield))
		{
			Action = EWandererAIAction::Defense;
		}
	}
	else
	{
		Action = EWandererAIAction::Attack;
	}

	Blackboard->SetValue<UBlackboardKeyType_Enum>(AIController->BBActionTypeKey, static_cast<uint8>(Action));
}
