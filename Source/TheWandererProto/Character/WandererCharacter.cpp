// Fill out your copyright notice in the Description page of Project Settings.


#include "WandererCharacter.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WandererCombatComponent.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "Player/WandererPlayerState.h"
#include "WandererDynamicCameraComponent.h"


AWandererCharacter::AWandererCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f; 
	SpringArmComponent->bUsePawnControlRotation = true; 

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); 
	CameraComponent->bUsePawnControlRotation = false;

	DynamicCameraComponent = CreateDefaultSubobject<UWandererDynamicCameraComponent>(TEXT("DynamicCameraSystem"));
	DynamicCameraComponent->SetupReferences(SpringArmComponent, CameraComponent);
}

void AWandererCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ASC 
	AWandererPlayerState* WandererPS = GetPlayerState<AWandererPlayerState>();
	check(WandererPS);
	
	WandererPS->GetAbilitySystemComponent()->InitAbilityActorInfo(WandererPS, this);
	AbilitySystemComponent = WandererPS->GetAbilitySystemComponent();
	HealthAttributeSet = WandererPS->GetHealthAttributeSet();
	CombatAttributeSet = WandererPS->GetCombatAttributeSet();
	CombatComponent->AssignAbilitySystemComponent(AbilitySystemComponent);
	
	GrantStartupAbilities();

	// Init Default Attributes By GE
	const UGameplayEffect* InitterGE = DefaultAttributesInitter->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(InitterGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

