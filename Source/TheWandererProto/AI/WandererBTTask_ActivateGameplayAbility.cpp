//  


#include "AI/WandererBTTask_ActivateGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "WandererAIController.h"
#include "AbilitySystem/Abilities/IRetriggerable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Character/Enemy/WandererEnemy.h"

UWandererBTTask_ActivateGameplayAbility::UWandererBTTask_ActivateGameplayAbility()
{
	NodeName = TEXT("Activate Gameplay Ability");
	bNotifyTick = true;
}

EBTNodeResult::Type UWandererBTTask_ActivateGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AWandererAIController* AIController = CastChecked<AWandererAIController>(OwnerComp.GetAIOwner());
	UAbilitySystemComponent* ASC = AIController->GetControllingEnemy()->GetAbilitySystemComponent();

	check(AbilityTag.IsValid());
	TArray<FGameplayAbilitySpecHandle> MatchingAbilitySpecs;
	ASC->FindAllAbilitiesWithTags(MatchingAbilitySpecs, FGameplayTagContainer(AbilityTag));
	check(MatchingAbilitySpecs.Num() == 1);
	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(MatchingAbilitySpecs[0]);
	
	if(AbilitySpec->IsActive())
	{
		IRetriggerable* RetriggerableAbility = Cast<IRetriggerable>(AbilitySpec->GetPrimaryInstance());
		if(RetriggerableAbility)
		{
			if(RetriggerableAbility->CanRetrigger())
			{
				RetriggerableAbility->SaveCurrentContext();
				return TryActivateAbility(ASC, AbilitySpec->Handle);
			}
			else
			{
				return EBTNodeResult::InProgress;
			}
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}
	else
	{
		return TryActivateAbility(ASC, AbilitySpec->Handle);
	}
}

void UWandererBTTask_ActivateGameplayAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	const AWandererAIController* AIController = CastChecked<AWandererAIController>(OwnerComp.GetAIOwner());
	UAbilitySystemComponent* ASC = AIController->GetControllingEnemy()->GetAbilitySystemComponent();
	TArray<FGameplayAbilitySpecHandle> MatchingAbilitySpecs;
	ASC->FindAllAbilitiesWithTags(MatchingAbilitySpecs, FGameplayTagContainer(AbilityTag));
	check(MatchingAbilitySpecs.Num() == 1);
	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(MatchingAbilitySpecs[0]);

	IRetriggerable* RetriggerableAbility = CastChecked<IRetriggerable>(AbilitySpec->GetPrimaryInstance());
	if(AbilitySpec->IsActive())
	{
		if(RetriggerableAbility->CanRetrigger())
		{
			RetriggerableAbility->SaveCurrentContext();
			if(ASC->TryActivateAbility(AbilitySpec->Handle))
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

EBTNodeResult::Type UWandererBTTask_ActivateGameplayAbility::TryActivateAbility(UAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& AbilityHandle)
{
	return ASC->TryActivateAbility(AbilityHandle) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
