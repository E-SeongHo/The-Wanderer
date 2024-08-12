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
class UMotionWarpingComponent;
class UInputMappingContext;
class UInputAction;
class UCharacterTrajectoryComponent;

UCLASS(Blueprintable)
class THEWANDERERPROTO_API AWandererCharacter : public AWandererBaseCharacter
{
	GENERATED_BODY()

public:
	AWandererCharacter();

	virtual void PossessedBy(AController* NewController) override;

	UMotionWarpingComponent* GetMotionWarpComponent() const { return MotionWarpComponent.Get(); }
	UCharacterTrajectoryComponent* GetTrajectoryComponent() const { return TrajectoryComponent.Get(); }
	
private:
	// Scene Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	// Actor Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DynamicCamera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWandererDynamicCameraComponent> DynamicCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionWarping, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trajectory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;
};
