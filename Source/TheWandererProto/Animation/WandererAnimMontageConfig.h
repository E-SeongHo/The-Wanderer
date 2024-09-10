//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WandererAnimMontageConfig.generated.h"

class UAnimMontage;
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

USTRUCT(BlueprintType)
struct FWandererActionMontageSequenceGroup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "ActionTag")) 
	FGameplayTag ActionTag;

	// Can also have specific tag
	// e.g) boss phase1, 2, ... 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWandererActionMontageGroup> MontageSequences;
};

USTRUCT(BlueprintType)
struct FWandererActionMontagePair
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> InstigatorMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> VictimMontage;
};

USTRUCT(BlueprintType)
struct FWandererActionMontagePairGroup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "ActionTag.Pair")) 
	FGameplayTag ActionTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWandererActionMontagePair> MontagePairs;
};

// This class is a wrapper to bind AnimMontage directly from the editor
// and at the same time to package this pair data into a GameplayEventData.OptionalObject.
// It is designed to avoid const_cast, ensuring that EventData contains the montage pair.
UCLASS()
class THEWANDERERPROTO_API UWandererMontagePair : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag ActionTag;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FWandererActionMontagePair Data;
};

UCLASS(Blueprintable)
class THEWANDERERPROTO_API UWandererAnimMontageConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	void MakeMontagePairCache();

	UAnimMontage* FindAnimMontageForTag(const FGameplayTag& ActionTag) const;

	UWandererMontagePair* FindAnimMontagePairForTag(const FGameplayTag& ActionTag) const;
	
	bool HasExactMatchingActionMontage(const FGameplayTag& ActionTag) const;

	bool HasComboDataSetForTag(const FGameplayTag& ActionTag) const;

	TArray<UAnimMontage*> FindComboMontageForTag(const FGameplayTag& ActionTag, const FGameplayTag& SpecificActionTag = FGameplayTag()) const;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "ActionTag", Categories = "ActionTag"))
	TArray<FWandererActionMontageSequenceGroup> ComboMontageGroups;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "ActionTag", Categories = "ActionTag"))
	TArray<FWandererActionMontageGroup> MontageGroups;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "ActionTag", Categories = "ActionTag.Pair"))
	TArray<FWandererActionMontagePairGroup> MontagePairGroups;

private:
	TMap<FGameplayTag, TObjectPtr<UWandererMontagePair>> PairWrapperCache;
};
