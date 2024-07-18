//  


#include "Character/WandererEnemy.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AWandererEnemy::AWandererEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	HealthAttributeSet = CreateDefaultSubobject<UWandererHealthAttributeSet>("Health AttributeSet");
	CombatAttributeSet = CreateDefaultSubobject<UWandererCombatAttributeSet>("Combat AttributeSet");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("UI");
	WidgetComponent->SetupAttachment(RootComponent);

	WidgetComponent->SetVisibility(false);
}

void AWandererEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(WidgetComponent->IsVisible())
	{
		// assuming single play
		const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		WidgetComponent->SetWorldRotation((CameraLocation - GetActorLocation()).Rotation());
	}
}

void AWandererEnemy::SetUIRender(bool bIsTargeting)
{
	WidgetComponent->SetVisibility(bIsTargeting);
}

void AWandererEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Init ASC
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	GrantStartupAbilities();
	
	// Default Gameplay Tag
	AbilitySystemComponent->AddLooseGameplayTag(WandererGameplayTags::State_Weapon_Unarmed);

	// Init Default Attributes By GE
	const UGameplayEffect* InitterGE = DefaultAttributesInitter->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(InitterGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
}
