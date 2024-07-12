//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/WandererAttributeSet.h"
#include "WandererHealthAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererHealthAttributeSet : public UWandererAttributeSet
{
	GENERATED_BODY()

public:
	UWandererHealthAttributeSet() : bOutOfHealth(false) {}
	
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, MaxHealth);

	// Meta Attributes
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Damage);
	
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	bool bOutOfHealth;
	
private:
	// Meta Attributes
	// Incoming healing amount
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;

	// Incoming damage amount
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};
