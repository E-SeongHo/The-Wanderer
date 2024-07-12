//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/WandererAnimNotifyState.h"
#include "WandererAnimNotifyState_MontageEvent.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAnimNotifyState_MontageEvent : public UWandererAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EventTag, Meta = (Categories = "Event"))
	FGameplayTag EventTag;
};
