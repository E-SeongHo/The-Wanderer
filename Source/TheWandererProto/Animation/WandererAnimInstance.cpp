// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "CharacterTrajectoryComponent.h"
#include "KismetAnimationLibrary.h"
#include "WandererAnimMontageConfig.h"
#include "WandererGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TheWandererProto/Character/WandererCharacter.h"

// This function 
// 1. When Derived ABP Created
// 2. When ABP Activated in Game 
void UWandererAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UWandererAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AWandererBaseCharacter* Character = Cast<AWandererBaseCharacter>(GetOwningActor());
	if (!Character) return;
	const UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!ASC) return;
	
	MovementComp = Character->GetCharacterMovement();
	Velocity = MovementComp->Velocity;
	GroundSpeed = Velocity.Size2D();
	GroundDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

	bShouldMove = GroundSpeed >= 3.0f && MovementComp->GetCurrentAcceleration().Length();
	bIsFalling = MovementComp->IsFalling();
	bIsCrouched = MovementComp->IsCrouching();
	bIsDefensing = ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Parry);
	bIsInCombat = ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat);

	CurrentCombatPoseSearchDatabase = CombatPoseSearchDatabases.Find(CurrentActiveWeaponSlot) ? CombatPoseSearchDatabases[CurrentActiveWeaponSlot] : nullptr; 
	bIsCombatPSDSet = CurrentCombatPoseSearchDatabase != nullptr;
	
	TrajectoryComp = Character->FindComponentByClass<UCharacterTrajectoryComponent>();
}

UAnimMontage* UWandererAnimInstance::GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const
{
	// check(MontageConfig->HasExactMatchingActionMontage(GameplayTag));
	return GetCurrentWeaponActionMontageConfig()->FindAnimMontageForTag(GameplayTag);
}

TArray<UAnimMontage*> UWandererAnimInstance::GetMatchingComboMontageForTag(const FGameplayTag& GameplayTag, const FGameplayTag& SpecificTag) const
{
	return GetCurrentWeaponActionMontageConfig()->FindComboMontageForTag(GameplayTag, SpecificTag);	
}

UWandererMontagePair* UWandererAnimInstance::GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const
{
	return GetCurrentWeaponActionMontageConfig()->FindAnimMontagePairForTag(GameplayTag);
}

void UWandererAnimInstance::OnActiveWeaponSlotChanged(const EWandererEquipmentSlot ActiveSlot)
{
	CurrentActiveWeaponSlot = ActiveSlot;
}

UWandererAnimMontageConfig* UWandererAnimInstance::GetCurrentWeaponActionMontageConfig() const
{
	check(!MontageConfigs.IsEmpty());
	check(MontageConfigs.Find(EWandererEquipmentSlot::Weapon1)); // needs at least one MontageSet
	
	UWandererAnimMontageConfig* MontageConfig = MontageConfigs.Find(CurrentActiveWeaponSlot) ? MontageConfigs[CurrentActiveWeaponSlot] : MontageConfigs[EWandererEquipmentSlot::Weapon1]; 
	check(MontageConfig);

	return MontageConfig;
}
