//  


#include "Character/WandererEnemy.h"

#include "AbilitySystemComponent.h"
#include "WandererCombatComponent.h"
#include "WandererGameplayTags.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "AI/WandererAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AWandererEnemy::AWandererEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	HealthAttributeSet = CreateDefaultSubobject<UWandererHealthAttributeSet>("Health AttributeSet");
	CombatAttributeSet = CreateDefaultSubobject<UWandererCombatAttributeSet>("Combat AttributeSet");

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("UI");
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetVisibility(false);
}

void AWandererEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AWandererAIController* WandererAIController = Cast<AWandererAIController>(NewController);
	check(WandererAIController);

	WandererAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	WandererAIController->RunBehaviorTree(BehaviorTree);
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

void AWandererEnemy::SetUIRender(bool bIsTargeted)
{
	WidgetComponent->SetVisibility(bIsTargeted);
}

void AWandererEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Init ASC
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	GrantStartupAbilities();

	// Init Default Attributes By GE
	const UGameplayEffect* InitterGE = DefaultAttributesInitter->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(InitterGE, 1.0f, AbilitySystemComponent->MakeEffectContext());

	CombatComponent->AssignAbilitySystemComponent(AbilitySystemComponent);
}
