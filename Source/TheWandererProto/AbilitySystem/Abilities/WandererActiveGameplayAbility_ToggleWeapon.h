//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_ToggleWeapon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_ToggleWeapon : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()

	UWandererActiveGameplayAbility_ToggleWeapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnDrawSword(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnSheathSword(FGameplayEventData Payload);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DrawSwordAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> SheathSwordAnim;
};
