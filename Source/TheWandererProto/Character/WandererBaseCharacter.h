// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Weapon/WandererEquipment.h"
#include "WandererBaseCharacter.generated.h"

class UWandererEquipmentComponent;
struct FGameplayTag;
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
class UWandererGameplayCueSoundConfig;

USTRUCT(BlueprintType)
struct FWandererEquipmentSpawnConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "AWandererEquipment"))
	TSubclassOf<AWandererEquipment> EquipmentType;

	// some can have DrawSocket, if it can be sheathed and drawn.
	// probably r_hand
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DrawSocket;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipSocket;
};

struct FWandererEquipmentSocketMap
{
	
};

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

	USoundBase* RequestSoundBaseForGameplayCue(const FGameplayTag& GameplayCueTag) const;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWandererEquipmentComponent> EquipmentComponent;
	
	// Attribute Set
	UPROPERTY(EditAnywhere, Category = Attributes)
	TSubclassOf<UGameplayEffect> DefaultAttributesInitter;

	// Auto Regen Health & Stamina Infinite GE
	UPROPERTY(EditAnywhere, Category = Attributes)
	TSubclassOf<UGameplayEffect> Regenerator;
	
	// Active Ability has Input Tag
	UPROPERTY(EditAnywhere, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWandererActiveGameplayAbility>> StartupActiveAbilities;

	UPROPERTY(EditAnywhere, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWandererGameplayAbility>> StartupTriggeredAbilities;

	// Sound
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWandererGameplayCueSoundConfig> GameplayCueSoundConfig;
	
	// AI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliComponent;
};
