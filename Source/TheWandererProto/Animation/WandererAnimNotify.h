//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WandererAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StateTag, Meta = (Categories = "State"))
	FGameplayTag StateTag;
};
