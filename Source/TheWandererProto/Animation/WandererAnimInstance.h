// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "Character/Component/WandererEquipmentComponent.h"
#include "WandererAnimInstance.generated.h"

class UPoseSearchDatabase;
struct FGameplayTag;
class UWandererMontagePair;
class UWandererAnimMontageConfig;
class AWandererCharacter;
class UCharacterMovementComponent;
class UCharacterTrajectoryComponent;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetMatchingMontageForTag(const FGameplayTag& GameplayTag) const;

	UFUNCTION(BlueprintCallable)
	TArray<UAnimMontage*> GetMatchingComboMontageForTag(const FGameplayTag& GameplayTag, const FGameplayTag& SpecificTag = FGameplayTag()) const;
	
	UFUNCTION(BlueprintCallable)
	UWandererMontagePair* GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const;

	void OnActiveWeaponSlotChanged(const EWandererEquipmentSlot ActiveSlot);
	
protected:
	UWandererAnimMontageConfig* GetCurrentWeaponActionMontageConfig() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TMap<EWandererEquipmentSlot, UWandererAnimMontageConfig*> MontageConfigs;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MotionMatching, Meta = (AllowPrivateAccess = true))
	TMap<EWandererEquipmentSlot, UPoseSearchDatabase*> CombatPoseSearchDatabases;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionMatching, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UPoseSearchDatabase> CurrentCombatPoseSearchDatabase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	EWandererEquipmentSlot CurrentActiveWeaponSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionMatching, Meta = (AllowPrivateAccess = true))
	bool bIsCombatPSDSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float GroundDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool bShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool bIsDefensing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	bool bIsInCombat;
};
