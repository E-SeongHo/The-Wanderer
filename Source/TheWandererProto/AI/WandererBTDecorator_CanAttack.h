//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "WandererBTDecorator_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTDecorator_CanAttack : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
