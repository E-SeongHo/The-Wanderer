//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "WandererGameplayAbility_Hit.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayAbility_Hit : public UWandererGameplayAbility
{
	GENERATED_BODY()

public:
	UWandererGameplayAbility_Hit();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USoundBase>> HitSounds;
};
