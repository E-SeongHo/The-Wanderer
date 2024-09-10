//  

#pragma once

#include "CoreMinimal.h"
#include "WandererActorComponent.h"
#include "WandererDynamicCameraComponent.generated.h"

class UWandererCombatComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererDynamicCameraComponent : public UWandererActorComponent
{
	GENERATED_BODY()

public:
	UWandererDynamicCameraComponent();

	void SetupReferences(USpringArmComponent* InSpringArmComponent, UCameraComponent* InCameraComponent);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	// Ownership is assigned to the owner
	// because I don't think having scene components within actor component does make sense
	TWeakObjectPtr<USpringArmComponent> SpringArmComponent;
	TWeakObjectPtr<UCameraComponent> CameraComponent;

	TWeakObjectPtr<AWandererBaseCharacter> Subject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MaxSpringArmLength = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (AllowPrivateAccess = "true"))
	float MinSpringArmLength = 400.0f;
};
