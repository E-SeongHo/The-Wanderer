// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WandererBaseCharacter.h"

#include "WandererCharacter.generated.h"

class UWandererDynamicCameraComponent;
class AWandererSword;
class AWandererWeapon;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable)
class THEWANDERERPROTO_API AWandererCharacter : public AWandererBaseCharacter
{
	GENERATED_BODY()

public:
	AWandererCharacter();

	virtual void PossessedBy(AController* NewController) override;

private:
	// Scene Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	// Actor Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DynamicCamera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWandererDynamicCameraComponent> DynamicCameraComponent;
};
