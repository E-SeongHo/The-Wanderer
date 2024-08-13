// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "WandererBaseCharacter.generated.h"

class UWandererCombatComponent;
class UWandererGameplayAbility;
class UWandererActiveGameplayAbility;
class AWandererWeapon;
class UGameplayEffect;
class UWandererHealthAttributeSet;
class UWandererCombatAttributeSet;
class UAIPerceptionStimuliSourceComponent;
class UMotionWarpingComponent;
class UCharacterTrajectoryComponent;

UCLASS()
class THEWANDERERPROTO_API AWandererBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWandererBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }

	UWandererCombatComponent* GetCombatComponent() const {return CombatComponent.Get(); }
	UWandererHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet.Get(); }
	UWandererCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet.Get(); }

	UMotionWarpingComponent* GetMotionWarpComponent() const { return MotionWarpComponent.Get(); }
	UCharacterTrajectoryComponent* GetTrajectoryComponent() const { return TrajectoryComponent.Get(); }

	virtual void Die();
	
protected:
	void GrantStartupAbilities();
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UWandererHealthAttributeSet> HealthAttributeSet;

	UPROPERTY()
	TObjectPtr<UWandererCombatAttributeSet> CombatAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWandererCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionWarping, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trajectory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;
	
	// Attribute Set
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributesInitter;
	
	// Active Ability has Input Tag
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWandererActiveGameplayAbility>> StartupActiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWandererGameplayAbility>> StartupTriggeredAbilities;

	// AI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliComponent;
};
