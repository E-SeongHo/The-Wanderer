//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_Parry.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Parry : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_Parry();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UFUNCTION()
	void OnParrySucceeded(FGameplayEventData Payload);

	UFUNCTION()
	void OnParryFailed(FGameplayEventData Payload);

	UFUNCTION()
	void ReleaseParry();

	void TriggerKnockBack(const FGameplayTag& ActionTag) const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USoundBase>> ParrySounds;
};


