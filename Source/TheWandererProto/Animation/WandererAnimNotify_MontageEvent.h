//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/WandererAnimNotify.h"
#include "WandererAnimNotify_MontageEvent.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAnimNotify_MontageEvent : public UWandererAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EventTag, Meta = (Categories = "Event"))
	FGameplayTag EventTag;
};
