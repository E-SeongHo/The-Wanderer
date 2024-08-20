//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WandererBTTask_StartCombat.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTTask_StartCombat : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UWandererBTTask_StartCombat();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
