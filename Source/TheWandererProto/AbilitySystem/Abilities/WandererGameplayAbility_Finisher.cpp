//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Finisher.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTagBase.h"
#include "Abilities/Tasks/AbilityTask_WaitOverlap.h"
#include "AbilitySystem/Effects/WandererGameplayEffect_Damage.h"
#include "Animation/WandererAnimMontageConfig.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/WandererAbilityTask_RepeatUntil.h"
#include "Weapon/WandererSword.h"

UWandererGameplayAbility_Finisher::UWandererGameplayAbility_Finisher()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_Finisher);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_Finisher);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::Ability_Attack);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = WandererGameplayTags::Event_Combat_Finisher;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UWandererGameplayAbility_Finisher::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AWandererCharacter* Instigator = Cast<AWandererCharacter>(GetAvatarActorFromActorInfo());
	const AWandererBaseCharacter* Target = Instigator->GetCombatComponent()->GetCombatTarget();
	check(Target);

	const FVector WarpLocation = Target->GetActorLocation() + Target->GetActorForwardVector() * 150.0f;
	const FVector WarpDirection = -Target->GetActorForwardVector();
	
	Instigator->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackTarget"), WarpLocation, WarpDirection.Rotation());

	UAnimMontage* MontageToPlay = Cast<UWandererMontagePair>(TriggerEventData->OptionalObject)->Data.InstigatorMontage;
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Finisher"), MontageToPlay);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Finisher::CallEndAbility);
	PlayMontageTask->ReadyForActivation();

	check(!Instigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Trace));
	
	UAbilityTask_WaitGameplayTagAdded* WaitWeaponTraceStarted = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, WandererGameplayTags::State_Weapon_Trace);
	WaitWeaponTraceStarted->Added.AddDynamic(this, &UWandererGameplayAbility_Finisher::StartSlowMotion);
	WaitWeaponTraceStarted->ReadyForActivation();

	// NOTE: The removed delegate will immediately broadcast since it is designed to check the tag upon activation.
	UAbilityTask_WaitGameplayTagRemoved* WaitWeaponTraceEnded = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, WandererGameplayTags::State_Weapon_Trace);
	WaitWeaponTraceEnded->Removed.AddDynamic(this, &UWandererGameplayAbility_Finisher::EndSlowMotion);
	WaitWeaponTraceEnded->ReadyForActivation();

	UGameplayStatics::SetGlobalTimeDilation(this, 0.8f);

	//TODO: Camera move, Disassemble?
}

void UWandererGameplayAbility_Finisher::StartSlowMotion()
{
	AWandererBaseCharacter* Instigator = CastChecked<AWandererBaseCharacter>(GetAvatarActorFromActorInfo());
	UGameplayStatics::SetGlobalTimeDilation(this, 0.3f);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Instigator->GetCombatComponent()->GetWeapon()->GetTraceSound(), Instigator->GetCombatComponent()->GetWeapon()->GetActorLocation());

	if(WeaponTraceRepeatTask) check(!WeaponTraceRepeatTask->IsActive());
	
	WeaponTraceRepeatTask = UWandererAbilityTask_RepeatUntil::RepeatAction(this, 0.01f);
	WeaponTraceRepeatTask->OnPerformAction.AddDynamic(this, &UWandererGameplayAbility_Finisher::OnWeaponTrace);
	WeaponTraceRepeatTask->ReadyForActivation();
}

void UWandererGameplayAbility_Finisher::OnWeaponTrace()
{
	AWandererWeapon* Weapon = (Cast<AWandererBaseCharacter>(GetAvatarActorFromActorInfo())->GetCombatComponent()->GetWeapon());
	FHitResult HitResult;
	const bool bHit = Weapon->Trace(HitResult);
	if(bHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Cyan, false, 1.0f);
		AWandererBaseCharacter* Target = CastChecked<AWandererBaseCharacter>(HitResult.GetActor());
		const UAbilitySystemComponent* InstigatorASC = GetAbilitySystemComponentFromActorInfo();
		
		const FGameplayAbilityTargetDataHandle TargetHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
		const FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(UWandererGameplayEffect_Damage::StaticClass(), GetAbilityLevel(), InstigatorASC->MakeEffectContext());
		
		// damage magnitude is irrelevant since the target is already dead.
		// I apply the damage effect primarily to easily pass the hit result to the gameplay cue.
		// if performance is concerned, this can be modified to execute the gameplay cue directly without using a gameplay effect.
		SpecHandle.Data->SetSetByCallerMagnitude(WandererGameplayTags::Data_Damage_Base, 2000.0f);

		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetHandle);
		WeaponTraceRepeatTask->ExternalCancel();
	}
}

void UWandererGameplayAbility_Finisher::EndSlowMotion()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 0.8f);

	if(WeaponTraceRepeatTask && WeaponTraceRepeatTask->IsActive()) WeaponTraceRepeatTask->ExternalCancel();
}

void UWandererGameplayAbility_Finisher::CallEndAbility()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

