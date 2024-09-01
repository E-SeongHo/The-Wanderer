//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "WandererActiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility : public UWandererGameplayAbility
{
	GENERATED_BODY()
	
public:
	UWandererActiveGameplayAbility() = default;
	UWandererActiveGameplayAbility(FGameplayTag Tag) : InputTag(Tag) {}
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};
