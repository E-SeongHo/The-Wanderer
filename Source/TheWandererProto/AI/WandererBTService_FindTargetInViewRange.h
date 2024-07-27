//  

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "WandererBTService_FindTargetInViewRange.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererBTService_FindTargetInViewRange : public UBTService
{
	GENERATED_BODY()

public:
	UWandererBTService_FindTargetInViewRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ViewAngle = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ViewDistance = 500.0f;
};
