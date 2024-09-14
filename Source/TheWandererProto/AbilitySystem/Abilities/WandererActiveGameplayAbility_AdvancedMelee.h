//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility_Melee.h"
#include "WandererActiveGameplayAbility_AdvancedMelee.generated.h"

/**
 * Provide Finisher, Counter, Dynamic Target Changing, Strong Attack(Chargeable Attack)
 */
UCLASS()
class THEWANDERERPROTO_API UWandererActiveGameplayAbility_AdvancedMelee : public UWandererActiveGameplayAbility_Melee
{
	GENERATED_BODY()

public:
	UWandererActiveGameplayAbility_AdvancedMelee();

	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
protected:
	virtual void DetermineAttackAction() override;
	
	virtual void SoftLock() override;

	virtual void ProcessHitSuccess(FHitResult& HitResult) override;
	virtual void ProcessHitBlocked(FHitResult& HitResult) override;
	virtual void ProcessHitMiss(FHitResult& HitResult) override;
	
private:
	void TriggerFinisher() const;

	UFUNCTION()
	void StartCharging();
	void ReleaseCharging();
	
	AWandererBaseCharacter* FindNearestOverlapTargetInDirection(const FVector& Direction, const float Angle, const float Distance) const;
	
private:
	bool bCanCounter;

	FTimerHandle ChargeTimer;
	float ChargedTime;
	float ChargeLimit = 1.5f;
};
