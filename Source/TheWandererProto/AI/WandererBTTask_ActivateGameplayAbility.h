//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WandererBTTask_ActivateGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTTask_ActivateGameplayAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UWandererBTTask_ActivateGameplayAbility();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FGameplayTag AbilityTag;
};
