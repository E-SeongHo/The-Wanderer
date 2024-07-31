//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "WandererBTDecorator_HasTag.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTDecorator_HasTag : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = GameplayTag)
	FGameplayTag GameplayTag;
};
