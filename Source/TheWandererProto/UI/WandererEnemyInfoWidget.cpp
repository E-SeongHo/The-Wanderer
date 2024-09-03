//  


#include "UI/WandererEnemyInfoWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "Character/Enemy/WandererEnemy.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UWandererEnemyInfoWidget::UWandererEnemyInfoWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWandererEnemyInfoWidget::AssignAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	Super::AssignAbilitySystemComponent(InASC);

	Name->SetText(FText::FromName(CastChecked<AWandererEnemy>(AbilitySystemComponent->GetAvatarActor())->GetName()));
	
	const UWandererHealthAttributeSet* HealthSet = Cast<UWandererHealthAttributeSet>(AbilitySystemComponent->GetAttributeSet(UWandererHealthAttributeSet::StaticClass()));
	HPBar->SetPercent(HealthSet->GetHealth() / HealthSet->GetMaxHealth());

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddLambda([this, HealthSet](const FOnAttributeChangeData& Data)
	{
		HPBar->SetPercent(HealthSet->GetHealth() / HealthSet->GetMaxHealth());
	});
}