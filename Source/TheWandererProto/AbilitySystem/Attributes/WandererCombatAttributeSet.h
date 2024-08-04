//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/WandererAttributeSet.h"
#include "WandererCombatAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererCombatAttributeSet : public UWandererAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UWandererCombatAttributeSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UWandererCombatAttributeSet, FinisherChance);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FinisherChance;
};
