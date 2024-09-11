//  


#include "AbilitySystem/Abilities/WandererActiveGameplayAbility_Melee.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Effects/WandererGameplayEffect_Damage.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/Component/WandererCombatComponent.h"
#include "Character/Component/WandererEquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/WandererAbilityTask_RepeatUntil.h"
#include "Weapon/WandererWeapon.h"

UWandererActiveGameplayAbility_Melee::UWandererActiveGameplayAbility_Melee()
{
	bRetriggerInstancedAbility = true;
}

void UWandererActiveGameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	SetupComboData();
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Combo index : %d"), ComboIndex));
	
	UAbilityTask_WaitGameplayTagAdded* WaitWeaponTrace = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, WandererGameplayTags::State_Weapon_Trace);
	WaitWeaponTrace->Added.AddDynamic(this, &UWandererActiveGameplayAbility_Melee::OnWeaponTraceStart);
	WaitWeaponTrace->ReadyForActivation();

	SetComboAvailable(false);
	bHasComboSaved = false;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UWandererActiveGameplayAbility_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	SetComboAvailable(false);
	if(bWasCancelled)
	{
		RemoveLooseTagFromOwner(WandererGameplayTags::State_Weapon_Trace);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UWandererActiveGameplayAbility_Melee::CanRetrigger() const
{
	check(bRetriggerInstancedAbility);
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Can retrigger")));

	return GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
}

void UWandererActiveGameplayAbility_Melee::SaveCurrentContext()
{
	IRetriggerable::SaveCurrentContext();
	bHasComboSaved = true;
}

void UWandererActiveGameplayAbility_Melee::DetermineAttackAction()
{
	if(DoesOwnerHaveTag(WandererGameplayTags::State_Stance_RightLead))
	{
		CurrentActionTag = WandererGameplayTags::ActionTag_Attack_RightLead;
	}
	else
	{
		CurrentActionTag =  WandererGameplayTags::ActionTag_Attack_LeftLead;
	}
}

void UWandererActiveGameplayAbility_Melee::SoftLock()
{
	const AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetCurrentActorInfo()->AvatarActor);
	check(Instigator);

	Instigator->GetMotionWarpComponent()->RemoveWarpTarget(TEXT("AttackTarget"));
	
	if(Instigator->GetCombatComponent()->IsTargetInAttackRange())
	{
		const AWandererBaseCharacter* CombatTarget = Instigator->GetCombatComponent()->GetCombatTarget();
		const FVector WarpDirection = (CombatTarget->GetActorLocation() - Instigator->GetActorLocation()).GetSafeNormal2D();
		const FVector WarpLocation = Instigator->GetDistanceTo(CombatTarget) > 150.0f ? CombatTarget->GetActorLocation() - WarpDirection * 150.0f : Instigator->GetActorLocation();

		//DrawDebugCircle(GetWorld(), WarpLocation, 30.0f, 10, FColor::Red, false, 3.0f);
		Instigator->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackTarget"), WarpLocation, WarpDirection.Rotation());
	}
	else
	{
		Super::SoftLock();
	}
}

void UWandererActiveGameplayAbility_Melee::ProcessAttack()
{
	// if pre-designed combo data exist, follow combo sequence
	if(ComboSequence.Num())
	{
		SoftLock();
		
		check(ComboIndex < ComboSequence.Num());
		PlayNewMontageTask(ComboSequence[ComboIndex++]);
		//CurrentPlayingMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Melee::ResetCombo);
	}
	// else choose random motion based on the CurrentActionTag
	else
	{
		Super::ProcessAttack();
	}
}


void UWandererActiveGameplayAbility_Melee::OnWeaponTraceStart()
{
	const AWandererWeapon* Weapon = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor)->FindComponentByClass<UWandererEquipmentComponent>()->GetCurrentWeapon();
	
	// since I didn't expand ASC to the weapon, just play the sound from the weapon
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Weapon->GetTraceSound(), Weapon->GetActorLocation());
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(WandererGameplayTags::GameplayCue_Sound_Attack);
	
	// Trace while WandererGameplayTags::State_Weapon_Trace is set or until hit something
	UWandererAbilityTask_RepeatUntil* WeaponTrace = UWandererAbilityTask_RepeatUntil::RepeatAction(this, 0.01f);
	WeaponTrace->OnPerformAction.AddDynamic(this, &UWandererActiveGameplayAbility_Melee::OnWeaponTrace);
	WeaponTrace->BindRequiredTag(WandererGameplayTags::State_Weapon_Trace);
	WeaponTrace->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Melee::OnWeaponTrace()
{
	const AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
	check(Instigator);
	UAbilitySystemComponent* InstigatorASC = Instigator->GetAbilitySystemComponent();
	
	FHitResult HitResult;
	const bool bHit = Instigator->FindComponentByClass<UWandererEquipmentComponent>()->GetCurrentWeapon()->Trace(HitResult);
	if(bHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Cyan, false, 1.0f);
		AWandererBaseCharacter* Target = CastChecked<AWandererBaseCharacter>(HitResult.GetActor());

		// Can Actually Hit?
		switch(EvaluateAttackResult(Target))
		{
		case EWandererAttackResult::Success:
			{
				const FGameplayAbilityTargetDataHandle TargetHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
				const FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(UWandererGameplayEffect_Damage::StaticClass(), GetAbilityLevel(), InstigatorASC->MakeEffectContext());

				const AWandererBaseCharacter* CauserWandererCharacter = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
				SpecHandle.Data->SetSetByCallerMagnitude(WandererGameplayTags::Data_Damage_Base, CauserWandererCharacter->GetCombatAttributeSet()->GetBaseDamage() * 0.25f);
		
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetHandle);

				break;	
			}
		case EWandererAttackResult::Blocked:
			{
				// To Target
				FGameplayEventData EventData;
				EventData.Instigator = Instigator;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, WandererGameplayTags::Event_Combat_ParryAttack, EventData);

				// To myself
				PlayNewMontageTask(GetMatchingMontageForTag(WandererGameplayTags::ActionTag_AttackFailed));
				SetComboAvailable(false);
				break;
			}
		case EWandererAttackResult::Miss:
			{
				break;
			}
		}

		InstigatorASC->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Trace);
		
		if(!InstigatorASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
		{
			Instigator->GetCombatComponent()->StartCombat();
		}
	}
}

void UWandererActiveGameplayAbility_Melee::SetComboAvailable(bool bIsAvailable)
{
	if(bIsAvailable)
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
	}
	else
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
	}
}

void UWandererActiveGameplayAbility_Melee::SetupComboData()
{
	// Get new pre-designed ComboSequence data if it exists
	if(!bHasComboSaved || bHasComboSaved && ComboSequence.Num() == ComboIndex)
	{
		ComboSequence = GetMatchingComboMontageForTag(WandererGameplayTags::ActionTag_Attack);
		ComboIndex = 0;
	}
}

void UWandererActiveGameplayAbility_Melee::ResetCombo()
{
	bHasComboSaved = false;
}
