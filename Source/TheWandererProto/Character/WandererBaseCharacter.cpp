 // Fill out your copyright notice in the Description page of Project Settings.


#include "WandererBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "CharacterTrajectoryComponent.h"
#include "MotionWarpingComponent.h"
#include "WandererCharacterMovementComponent.h"
#include "WandererCombatComponent.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "AbilitySystem/Cues/WandererGameplayCueSoundConfig.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Weapon/WandererWeapon.h"

 AWandererBaseCharacter::AWandererBaseCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UWandererCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CombatComponent = CreateDefaultSubobject<UWandererCombatComponent>(TEXT("Combat Component"));
	
	StimuliComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("StimuliSource Component");
	StimuliComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliComponent->RegisterWithPerceptionSystem();

 	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
 	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("Trajectory"));
}

 USoundBase* AWandererBaseCharacter::RequestSoundBaseForGameplayCue(const FGameplayTag& GameplayCueTag) const
 {
 	return GameplayCueSoundConfig->FindSoundBaseForGameplayCueTag(GameplayCueTag);
 }

void AWandererBaseCharacter::Die()
{
 	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
 	GetCharacterMovement()->GravityScale = 0;
 	GetCharacterMovement()->Velocity = FVector(0);
 	
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
 	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->SetSimulatePhysics(true);
}

void AWandererBaseCharacter::GrantStartupAbilities()
{
	// Grant Startup Active Abilities
	// I'm considering it might be better if it happens in ASC class?
	for(auto ActiveAbility : StartupActiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ActiveAbility, 1);

		if(const UWandererActiveGameplayAbility* WandererActiveGA = Cast<UWandererActiveGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(WandererActiveGA->InputTag);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	// Grant Startup Triggered Abilities ( until now, it just means all abilities which doesn't need an input )
	for(auto TriggeredAbility : StartupTriggeredAbilities)
	{
 		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(TriggeredAbility, 1);

 		if(const UWandererGameplayAbility* WandererGA = Cast<UWandererGameplayAbility>(AbilitySpec.Ability))
 		{
 			AbilitySystemComponent->GiveAbility(AbilitySpec);
 		}
	}
}



