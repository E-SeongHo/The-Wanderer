//  

#pragma once

#include "CoreMinimal.h"
#include "UI/WandererWidget.h"
#include "WandererEnemyInfoWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererEnemyInfoWidget : public UWandererWidget
{
	GENERATED_BODY()
	
public:
	UWandererEnemyInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void AssignAbilitySystemComponent(UAbilitySystemComponent* InASC) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;
};
