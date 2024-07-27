//  

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WandererAIController.generated.h"

struct FBlackboardKeySelector;
struct FAIStimulus;
class UBehaviorTreeComponent;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API AWandererAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWandererAIController();
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
