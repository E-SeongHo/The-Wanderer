//  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "WandererPlayerState.generated.h"


class UWandererHealthAttributeSet;
class UWandererCombatAttributeSet;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API AWandererPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWandererPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }
	UWandererHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet.Get(); }
	UWandererCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet.Get(); }
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UWandererHealthAttributeSet> HealthAttributeSet;

	UPROPERTY()
	TObjectPtr<UWandererCombatAttributeSet> CombatAttributeSet;
};
