//  


#include "WandererBTService_FindTargetInViewRange.h"

#include "Character/WandererEnemy.h"
#include "WandererAIController.h"
#include "Character/WandererCharacter.h"
#include "Utility/WandererUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

UWandererBTService_FindTargetInViewRange::UWandererBTService_FindTargetInViewRange()
{
	
}

void UWandererBTService_FindTargetInViewRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AWandererEnemy* ControllingEnemy = Cast<AWandererEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	check(ControllingEnemy);

	const TArray<AActor*> Targets = WandererUtils::FindOverlappingActorsInViewRange(AWandererCharacter::StaticClass(), ControllingEnemy, ViewAngle, ViewDistance, ECC_GameTraceChannel1);

	// since it's designed as a single player game
	check(Targets.Num() == 1 || Targets.Num() == 0); 

	if(Targets.Num())
	{
		AWandererCharacter* Target = Cast<AWandererCharacter>(Targets[0]);
		check(Target);

		// OwnerComp.GetBlackboardComponent()->SetValueAsObject(, Target);
	}
	
}
