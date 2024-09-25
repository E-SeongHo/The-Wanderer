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
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Resilience);
	
	// Meta Attributes
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Healing);
	ATTRIBUTE_ACCESSORS(UWandererHealthAttributeSet, Damage);
	
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	void ProcessDamageEffect(const FGameplayEffectModCallbackData& Data);
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	bool bOutOfHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Resilience;
	
private:
	// Meta Attributes
	// Incoming healing amount
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;

	// Incoming damage amount
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};
