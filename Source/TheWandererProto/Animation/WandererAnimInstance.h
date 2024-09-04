// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
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
	UWandererMontagePair* GetMatchingMontagePairForTag(const FGameplayTag& GameplayTag) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UWandererAnimMontageConfig> MontageConfig;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MotionMatching, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UPoseSearchDatabase> CombatPoseSearchDatabase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MotionMatching, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UPoseSearchDatabase> DefensedPoseSearchDatabase;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
	float GroundSpeed;

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
