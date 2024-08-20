//  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WandererCharacterMovementComponent.generated.h"

UENUM()
enum class EWandererUprightMovement
{
	Run,
	Walk,
	Sprint,
};

class USoundCue;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UWandererCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;

	void StartSprinting();
	void StopSprinting();

	void StartWalking();
	void StopWalking();

	UFUNCTION(BlueprintCallable)
	void OnFoleyMovementEvent();

private:
	void SetUprightLocomotionMode(const EWandererUprightMovement InMode);
 
private:
	EWandererUprightMovement UprightLocomotionMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Locomotion, meta = (AllowPrivateAccess = "true"))
	EWandererUprightMovement DefaultUprightLocomotion = EWandererUprightMovement::Run;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FoleySound, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundCue> FootStepSoundCue;
};
