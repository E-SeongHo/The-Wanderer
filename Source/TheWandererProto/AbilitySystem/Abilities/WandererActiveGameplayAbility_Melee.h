//  

#pragma once

#include "CoreMinimal.h"
#include "IRetriggerable.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility_Attack.h"
#include "WandererActiveGameplayAbility_Melee.generated.h"

/**
 * 
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_Melee : public UWandererActiveGameplayAbility_Attack, public IRetriggerable
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_Melee();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	// IRetriggerable
	virtual bool CanRetrigger() const override;
	virtual void SaveCurrentContext() override;
	
protected:
	virtual void DetermineAttackAction() override;

	virtual void SoftLock() override;

	virtual void ProcessAttack() override;
	
	UFUNCTION()
	void OnWeaponTraceStart();

	UFUNCTION()
	void OnWeaponTrace();

	virtual void ProcessHitSuccess(FHitResult& HitResult);
	virtual void ProcessHitBlocked(FHitResult& HitResult);
	virtual void ProcessHitMiss(FHitResult& HitResult);
	
	void SetComboAvailable(bool bIsAvailable);

	virtual void SetupComboData();

	UFUNCTION()
	void ResetCombo();

protected:
	TArray<UAnimMontage*> ComboSequence;
	uint8 ComboIndex;
	bool bHasComboSaved;
	
	uint16 TotalComboCount;
};
