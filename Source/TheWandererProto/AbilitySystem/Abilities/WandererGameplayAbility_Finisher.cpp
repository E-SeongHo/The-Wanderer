//  


#include "AbilitySystem/Abilities/WandererGameplayAbility_Finisher.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MotionWarpingComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/WandererAnimMontageConfig.h"
#include "Character/WandererCharacter.h"
#include "Character/WandererCombatComponent.h"
#include "Kismet/GameplayStatics.h"

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
	const FVector WarpDirection = Target->GetActorLocation() - Instigator->GetActorLocation();
	
	Instigator->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackTarget"), WarpLocation, WarpDirection.Rotation());

	UAnimMontage* MontageToPlay = Cast<UWandererMontagePair>(TriggerEventData->OptionalObject)->Data.InstigatorMontage;
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Finisher"), MontageToPlay);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererGameplayAbility_Finisher::CallEndAbility);
	PlayMontageTask->ReadyForActivation();

	//UGameplayStatics::SetGlobalTimeDilation(this, f);
	//TODO: Camera move, blood trails, Damage, Disassemble?
}

void UWandererGameplayAbility_Finisher::CallEndAbility()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

