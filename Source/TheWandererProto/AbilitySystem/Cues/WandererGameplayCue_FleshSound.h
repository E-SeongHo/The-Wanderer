//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "WandererGameplayCue_FleshSound.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayCue_FleshSound : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USoundBase>> FleshSounds;
};
