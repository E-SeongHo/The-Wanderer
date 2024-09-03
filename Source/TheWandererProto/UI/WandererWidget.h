//  

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WandererWidget.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWandererWidget(const FObjectInitializer& ObjectInitializer);

	virtual void AssignAbilitySystemComponent(UAbilitySystemComponent* InASC);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))	
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
