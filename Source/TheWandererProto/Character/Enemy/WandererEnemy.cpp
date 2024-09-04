//  


#include "WandererEnemy.h"

#include "AbilitySystemComponent.h"
#include "Character/WandererCombatComponent.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WandererEnemyInfoWidget.h"

AWandererEnemy::AWandererEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	HealthAttributeSet = CreateDefaultSubobject<UWandererHealthAttributeSet>("Health AttributeSet");
	CombatAttributeSet = CreateDefaultSubobject<UWandererCombatAttributeSet>("Combat AttributeSet");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;

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

void AWandererEnemy::Die()
{
	Super::Die();
	DetachFromControllerPendingDestroy();
}

void AWandererEnemy::SetUIRender(bool bIsTargeted)
{
	WidgetComponent->SetVisibility(bIsTargeted);
}

FName AWandererEnemy::GetName() const
{
	return Name;
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
	CastChecked<UWandererEnemyInfoWidget>(WidgetComponent->GetUserWidgetObject())->AssignAbilitySystemComponent(AbilitySystemComponent);
}
