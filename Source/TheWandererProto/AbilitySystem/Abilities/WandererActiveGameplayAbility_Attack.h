//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "WandererActiveGameplayAbility_Attack.generated.h"

UENUM()
enum class EWandererAttackResult : uint8
{
	Success,
	Miss,
	Block,
};

class AWandererBaseCharacter;
class UAbilityTask_PlayMontageAndWait;
class UNiagaraSystem;
/**
 * 
 */
UCLASS(Blueprintable)
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Attack : public UWandererActiveGameplayAbility
{
	GENERATED_BODY()
public:
	UWandererActiveGameplayAbility_Attack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	void SoftLock();
	
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnComboAvailable(FGameplayEventData Payload);

	UFUNCTION()
	void OnWeaponTraceStart(FGameplayEventData Payload);

	UFUNCTION()
	void OnWeaponTrace();

	UFUNCTION()
	bool ShouldStopWeaponTrace();

	EWandererAttackResult EvaluateAttackResult(AWandererBaseCharacter* Target);

	void PlayNewMontageForTag(const FGameplayTag& GameplayTag);
	void SetComboAvailable(bool bIsAvailable);
	bool IsComboAvailable() const;
	
private:
	TObjectPtr<UAbilityTask_PlayMontageAndWait> CurrentPlayingMontageTask;
	int32 ComboCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blood, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> BloodEffect;
};
