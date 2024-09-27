//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "WandererBTService_DetermineAction_Melee.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTService_DetermineAction_Melee : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UWandererBTService_DetermineAction_Melee();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
