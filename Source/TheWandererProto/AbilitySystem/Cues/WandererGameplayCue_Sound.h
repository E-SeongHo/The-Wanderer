//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "WandererGameplayCue_Sound.generated.h"


UENUM(BlueprintType)
enum class EWandererSoundPlayLocation : uint8
{
	Instigator,
	HitTarget,
	HitImpactPoint,
	None,
};
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayCue_Sound : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	EWandererSoundPlayLocation SoundPlayLocationType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> SoundBase;
};
