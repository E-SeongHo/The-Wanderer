//  

#pragma once

#include "CoreMinimal.h"
#include "UI/WandererWidget.h"
#include "WandererPlayerWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererPlayerWidget : public UWandererWidget
{
	GENERATED_BODY()
	
public:
	UWandererPlayerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void AssignAbilitySystemComponent(UAbilitySystemComponent* InASC) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
};
