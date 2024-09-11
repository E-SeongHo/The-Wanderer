//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "WandererBTTask_ActivateGameplayAbility.generated.h"

struct FGameplayAbilitySpecHandle;
class UAbilitySystemComponent;
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
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type TryActivateAbility(UAbilitySystemComponent* ASC, const FGameplayAbilitySpecHandle& AbilityHandle);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FGameplayTag AbilityTag;
};
