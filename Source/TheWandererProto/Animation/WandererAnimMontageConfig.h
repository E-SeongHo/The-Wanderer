//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WandererAnimMontageConfig.generated.h"


// Struct used to centralize the retrieval of character's appropriate animation montage 
// that is bound to a specific action
USTRUCT(BlueprintType)
struct FWandererActionMontageGroup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "ActionTag")) 
	FGameplayTag ActionTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> Montages;
};

/**
 * 
 */
UCLASS(Blueprintable)
class THEWANDERERPROTO_API UWandererAnimMontageConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UAnimMontage* FindAnimMontageForTag(const FGameplayTag& ActionTag) const;

	bool HasExactMatchingActionMontage(const FGameplayTag& ActionTag) const;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "ActionTag", Categories = "ActionTag"))
	TArray<FWandererActionMontageGroup> MontageGroups;
};
