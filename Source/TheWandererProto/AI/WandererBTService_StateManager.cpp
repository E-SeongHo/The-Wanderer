//  


#include "WandererBTService_StateManager.h"

UWandererBTService_StateManager::UWandererBTService_StateManager()
{
	NodeName = TEXT("AI State Manager");
	RandomDeviation = 1.0f;
}

void UWandererBTService_StateManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
}

