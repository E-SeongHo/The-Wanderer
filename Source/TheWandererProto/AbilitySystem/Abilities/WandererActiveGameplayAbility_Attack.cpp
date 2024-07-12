//  


#include "WandererActiveGameplayAbility_Attack.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "Weapon/WandererSword.h"

UWandererActiveGameplayAbility_Attack::UWandererActiveGameplayAbility_Attack() : Super(WandererGameplayTags::InputTag_Attack)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UWandererActiveGameplayAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Unarmed)) return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Generate AbilityTask : Play Montage
	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), AttackAnims[FMath::RandRange(0, AttackAnims.Num()-1)]);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
	PlayMontageTask->ReadyForActivation();

	// Generate AbilityTask : Wait Event (Event.Montage.ComboAvailable)
	{
		UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_ComboAvailable);
		WaitEventTask->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnComboAvailable);
		WaitEventTask->ReadyForActivation();
	}

	// Generate AbilityTask : Wait Event (Event.Montage.WeaponTrace)
	{
		if(CurrentActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Sword))
		{
			UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_WeaponTrace);
			WaitEventTask->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnWeaponTrace);
			WaitEventTask->ReadyForActivation();
		}
	}
}

void UWandererActiveGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ComboCount = 0;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if(IsActive() && bIsComboAvailable) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Combo resume")));

		PlayMontageTask->ExternalCancel();

		PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), AttackAnims[FMath::RandRange(0, AttackAnims.Num()-1)]);
		PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
		PlayMontageTask->ReadyForActivation();
		
		ComboCount++;
		bIsComboAvailable = false;
	}
}

void UWandererActiveGameplayAbility_Attack::OnMontageCompleted()
{
	bIsComboAvailable = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Attack::OnComboAvailable(FGameplayEventData Payload)
{
	bIsComboAvailable = true;
}

void UWandererActiveGameplayAbility_Attack::OnWeaponTrace(FGameplayEventData Payload)
{
	// Trace until WandererGameplayTags::State_Weapon_Trace is set or until hit something
	FTimerHandle WeaponTraceTimer;
	GetWorld()->GetTimerManager().SetTimer(WeaponTraceTimer, [this, &WeaponTraceTimer]()
	{
		UAbilitySystemComponent* CauserASC = this->GetActorInfo().AbilitySystemComponent.Get();
		if(CauserASC->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Trace))
		{
			const AWandererBaseCharacter* WandererBaseCharacter = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
			check(WandererBaseCharacter);
			
			FHitResult HitResult;
			bool bHit = WandererBaseCharacter->GetWeapon()->Trace(HitResult);
			if(bHit)
			{
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Cyan, false, 1.0f);

				const AWandererBaseCharacter* TargetWandererCharacter = Cast<AWandererBaseCharacter>(HitResult.GetActor());
				if(TargetWandererCharacter)
				{
					// 1. Damage Gameplay Effect
					check(DamageEffect);
					const FGameplayAbilityTargetDataHandle TargetHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
					const FGameplayEffectSpecHandle SpecHandle = CauserASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), CauserASC->MakeEffectContext());

					const AWandererBaseCharacter* CauserWandererCharacter = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
					SpecHandle.Data->SetSetByCallerMagnitude(WandererGameplayTags::Data_Damage_Base, CauserWandererCharacter->GetCombatAttributeSet()->GetBaseDamage());
					
					ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetHandle);
					// ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetHandle, DamageEffect, GetAbilityLevel());
					
					// TODO:
					// 2. Blood Effect
					// 3. Sound
					// 4. Animation

					CauserASC->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Trace);
					GetWorld()->GetTimerManager().ClearTimer(WeaponTraceTimer);
				}
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(WeaponTraceTimer);
		}
	}, 0.1f, true);
}


