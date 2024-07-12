//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WandererAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StateTag, Meta = (Categories = "State"))
	FGameplayTag StateTag;
};
