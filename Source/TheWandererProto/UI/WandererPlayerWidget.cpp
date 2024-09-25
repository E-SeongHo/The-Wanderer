//  


#include "UI/WandererPlayerWidget.h"

#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "Components/ProgressBar.h"

UWandererPlayerWidget::UWandererPlayerWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWandererPlayerWidget::AssignAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	Super::AssignAbilitySystemComponent(InASC);

	const UWandererHealthAttributeSet* HealthSet = Cast<UWandererHealthAttributeSet>(AbilitySystemComponent->GetAttributeSet(UWandererHealthAttributeSet::StaticClass()));

	HPBar->SetPercent(HealthSet->GetHealth() / HealthSet->GetMaxHealth());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddLambda([this, HealthSet](const FOnAttributeChangeData& Data)
	{
		HPBar->SetPercent(HealthSet->GetHealth() / HealthSet->GetMaxHealth());
	});

	StaminaBar->SetPercent(HealthSet->GetStamina() / HealthSet->GetMaxStamina());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetStaminaAttribute()).AddLambda([this, HealthSet](const FOnAttributeChangeData& Data)
	{
		StaminaBar->SetPercent(HealthSet->GetStamina() / HealthSet->GetMaxStamina());
	});
}
