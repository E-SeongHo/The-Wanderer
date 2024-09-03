//  


#include "UI/WandererWidget.h"

UWandererWidget::UWandererWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWandererWidget::AssignAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;
}
