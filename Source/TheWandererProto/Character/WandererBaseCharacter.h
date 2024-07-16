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

UCLASS()
class THEWANDERERPROTO_API AWandererBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AWandererBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }

	UWandererCombatComponent* GetCombatComponent() const {return CombatComponent.Get(); }
	UWandererHealthAttributeSet* GetHealthAttributeSet() const { return HealthAttributeSet.Get(); }
	UWandererCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet.Get(); }

	AWandererWeapon* GetWeapon() const { return Weapon.Get(); }
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
	
	// Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWandererWeapon> Weapon;

	// Attribute Set
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributesInitter;
	
	// Active Ability has Input Tag
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWandererActiveGameplayAbility>> StartupActiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UWandererGameplayAbility>> StartupTriggeredAbilities;
};
