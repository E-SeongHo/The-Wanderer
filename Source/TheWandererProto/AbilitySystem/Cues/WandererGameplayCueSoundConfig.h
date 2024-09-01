//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WandererGameplayCueSoundConfig.generated.h"

USTRUCT(BlueprintType)
struct FWandererGameplayCueSound
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "GameplayCue")) 
	FGameplayTag GameplayCueTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> SoundBase;	
};

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayCueSoundConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	USoundBase* FindSoundBaseForGameplayCueTag(const FGameplayTag& GameplayCueTag) const;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "GameplayCueSound"))
	TArray<FWandererGameplayCueSound> GameplayCueSounds;
};
