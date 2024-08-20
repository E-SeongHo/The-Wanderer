//  

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "Perception/AIPerceptionTypes.h"
#include "WandererAIController.generated.h"


UENUM(BlueprintType)
enum class EWandererAIBehavior : uint8
{
	Idle,
	Wait,
	Patrol,
	Avoid,
	Parry,
	Approach,
	Attack,
};

class AWandererEnemy;
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
	AWandererEnemy* GetControllingEnemy() const;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual bool InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset) override;
	
public:
	FBlackboard::FKey BBSightTargetKey;
	FBlackboard::FKey BBSoundLocationKey;
	FBlackboard::FKey BBCombatTargetLocationKey;
	FBlackboard::FKey BBBehaviorTypeKey;
	
protected:
	UPROPERTY(EditAnywhere, Category = Asset, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	TWeakObjectPtr<AWandererEnemy> ControllingEnemy;
	
private:
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

	void HandleSight(AActor* Actor, FAIStimulus Stimulus);
	void HandleHearing(AActor* Actor, FAIStimulus Stimulus);
};
