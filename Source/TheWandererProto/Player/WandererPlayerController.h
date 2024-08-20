// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTags.h"
#include "WandererPlayerController.generated.h"

class UAbilitySystemComponent;
class UWandererInputConfig;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class THEWANDERERPROTO_API AWandererPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// Native Input Actions
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Crouch(const FInputActionValue& Value);
	void Input_UnCrouch(const FInputActionValue& Value);

	// Ability Input Actions
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	void Input_AbilityInputTagHeld(FGameplayTag InputTag);

	UAbilitySystemComponent* GetAbilitySystemComponent() const;
private:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> WandererMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UWandererInputConfig> InputConfig;
	
};
