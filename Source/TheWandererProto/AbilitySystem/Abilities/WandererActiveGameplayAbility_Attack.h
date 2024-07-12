//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_Attack.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS(Blueprintable)
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Attack : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()
public:
	UWandererActiveGameplayAbility_Attack();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnComboAvailable(FGameplayEventData Payload);

	UFUNCTION()
	void OnWeaponTrace(FGameplayEventData Payload);
		
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DamageEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AttackAnims;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;
	bool bIsComboAvailable = false;
	int32 ComboCount = 0;
};
