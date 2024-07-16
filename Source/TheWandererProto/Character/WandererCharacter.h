// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WandererBaseCharacter.h"
#include "GameFramework/Character.h"
#include "WandererBaseCharacter.h"

#include "WandererCharacter.generated.h"

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
	virtual void Tick(float DeltaTime) override;

	UMotionWarpingComponent* GetMotionWarpComponent() const { return MotionWarpComponent.Get(); }
	UCharacterTrajectoryComponent* GetTrajectoryComponent() const { return TrajectoryComponent.Get(); }

	void AttachWeaponMeshToSocket(FName SocketName);
	
protected:
	virtual void BeginPlay() override;

private:
	// Scene Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	// Actor Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionWarping, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trajectory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterTrajectoryComponent> TrajectoryComponent;
 
	// Set in BluePrint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWandererSword> SwordType;

};
