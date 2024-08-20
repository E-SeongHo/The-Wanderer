//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "WandererBTService_StateManager.generated.h"

enum class EWandererAIBehavior : uint8;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTService_StateManager : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UWandererBTService_StateManager();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
