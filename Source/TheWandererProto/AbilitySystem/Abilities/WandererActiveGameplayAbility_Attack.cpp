//  


#include "WandererActiveGameplayAbility_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Effects/WandererGameplayEffect_Damage.h"
#include "Character/WandererCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/WandererAbilityTask_RepeatUntil.h"
#include "Tasks/WandererAbilityTask_SmoothRotate.h"
#include "Utility/WandererUtils.h"
#include "Weapon/WandererSword.h"

UWandererActiveGameplayAbility_Attack::UWandererActiveGameplayAbility_Attack()
	: Super(WandererGameplayTags::InputTag_Attack)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_Attack);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_Attack);
	
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Draw);
	CancelAbilitiesWithTag.AddTag(WandererGameplayTags::State_Parry);
}


void UWandererActiveGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//SoftLock();
	
	CurrentPlayingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), AttackAnimsFromLeftLead[FMath::RandRange(0, AttackAnimsFromLeftLead.Num()-1)]); 
	CurrentPlayingMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
	CurrentPlayingMontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitComboAvailable = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_ComboAvailable);
	WaitComboAvailable->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnComboAvailable);
	WaitComboAvailable->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitWeaponTrace = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_WeaponTrace);
	WaitWeaponTrace->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnWeaponTraceStart);
	WaitWeaponTrace->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ComboCount = 0;
	SetComboAvailable(false);

	if(bWasCancelled)
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Trace);
		CurrentPlayingMontageTask->ExternalCancel();
	}

	if(Cast<AWandererCharacter>(ActorInfo->AvatarActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("End Attack %d"), bWasCancelled));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	// Perform combo attack if the input has pressed in right timing 
	if(IsActive() && IsComboAvailable()) 
	{
		CurrentPlayingMontageTask->ExternalCancel();
		
		//SoftLock();
		UAnimMontage* MontageToPlay;
		if(ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Stance_RightLead))
		{
			MontageToPlay = AttackAnimsFromRightLead[FMath::RandRange(0, AttackAnimsFromRightLead.Num()-1)];
		}
		else
		{
			MontageToPlay = AttackAnimsFromLeftLead[FMath::RandRange(0, AttackAnimsFromLeftLead.Num()-1)];
		}
		
		CurrentPlayingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), MontageToPlay);
		CurrentPlayingMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
		CurrentPlayingMontageTask->ReadyForActivation();
		
		ComboCount++;
		SetComboAvailable(false);
	}
}

void UWandererActiveGameplayAbility_Attack::SoftLock()
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetCurrentActorInfo()->AvatarActor);
	check(Instigator);
	
	TArray<AActor*> OverlapTargets = WandererUtils::FindOverlappingActorsInViewRange(AWandererBaseCharacter::StaticClass(), Instigator, 120.0f, 200.0f, ECC_GameTraceChannel1);
	
	AActor* NearestActor = nullptr;
	const FVector Location = Instigator->GetActorLocation();
	float MinDist = 9999.0f;
	
	for(AActor* Target : OverlapTargets)
	{
		const float Dist = FVector::DistXY(Target->GetActorLocation(), Location);
		if(Dist < MinDist)
		{
			MinDist = Dist;
			NearestActor = Target;
		}
	}

	if(NearestActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlap detected!")));

		FVector Direction = NearestActor->GetActorLocation() - Location;
		Direction = Direction.GetSafeNormal2D();

		// subtle adjustment 
		Instigator->SetActorRotation(Direction.Rotation());
		Instigator->SetActorLocation(NearestActor->GetActorLocation() - Direction * 100.0f);

		if(!Instigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
		{
			Instigator->GetController()->SetControlRotation(Direction.Rotation());
		}
		//Instigator->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackTarget"), NearestActor->GetActorLocation() - Direction * 100.0f, Direction.Rotation());
	}
	else
	{
		// Attack orientation
		if(!Instigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
		{
			UWandererAbilityTask_SmoothRotate* SmoothRotator = UWandererAbilityTask_SmoothRotate::SmoothRotate(this, Instigator->GetActorRotation(), Instigator->GetControlRotation());
			//SmoothRotator->ReadyForActivation();	
		}
		//Instigator->GetMotionWarpComponent()->RemoveWarpTarget(TEXT("AttackTarget"));
	}
}

void UWandererActiveGameplayAbility_Attack::OnMontageCompleted()
{
	SetComboAvailable(false);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Attack::OnComboAvailable(FGameplayEventData Payload)
{
	SetComboAvailable(true);
}

void UWandererActiveGameplayAbility_Attack::OnWeaponTraceStart(FGameplayEventData Payload)
{
	const AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
	check(Instigator);
	UAbilitySystemComponent* InstigatorASC = Instigator->GetAbilitySystemComponent();
	
	// basic tracing sound of the weapon (don't need to replicate)
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Instigator->GetCombatComponent()->GetWeapon()->GetTraceSound(), Instigator->GetCombatComponent()->GetWeapon()->GetActorLocation());
	
	// Trace while WandererGameplayTags::State_Weapon_Trace is set or until hit something
	UWandererAbilityTask_RepeatUntil* WeaponTrace = UWandererAbilityTask_RepeatUntil::RepeatAction(this, 0.01f);
	WeaponTrace->OnPerformAction.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnWeaponTrace);
	WeaponTrace->OnCycleEndConditionCheck.BindDynamic(this, &UWandererActiveGameplayAbility_Attack::ShouldStopWeaponTrace);
	WeaponTrace->ReadyForActivation();
}

void UWandererActiveGameplayAbility_Attack::OnWeaponTrace()
{
	const AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
	check(Instigator);
	UAbilitySystemComponent* InstigatorASC = Instigator->GetAbilitySystemComponent();
	
	FHitResult HitResult;
	const bool bHit = Instigator->GetCombatComponent()->GetWeapon()->Trace(HitResult);
	if(bHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Cyan, false, 1.0f);
		AWandererBaseCharacter* Target = CastChecked<AWandererBaseCharacter>(HitResult.GetActor());

		// Can Actually Hit?
		switch(EvaluateAttackResult(Target))
		{
		case EWandererAttackResult::Success:
			{
				// 1. Damage Gameplay Effect
				const FGameplayAbilityTargetDataHandle TargetHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
				const FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(UWandererGameplayEffect_Damage::StaticClass(), GetAbilityLevel(), InstigatorASC->MakeEffectContext());

				const AWandererBaseCharacter* CauserWandererCharacter = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
				SpecHandle.Data->SetSetByCallerMagnitude(WandererGameplayTags::Data_Damage_Base, CauserWandererCharacter->GetCombatAttributeSet()->GetBaseDamage() * 0.25f);
		
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetHandle);
				// ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetHandle, DamageEffect, GetAbilityLevel());

				// 2. Blood Effect TODO: Gameplay Cue
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, HitResult.ImpactPoint);
				break;	
			}
		case EWandererAttackResult::Block:
			{
				// To Target
				FGameplayEventData EventData;
				EventData.Instigator = Instigator;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, WandererGameplayTags::Event_Combat_ParryAttack, EventData);

				// To myself
				CurrentPlayingMontageTask->ExternalCancel();
				CurrentPlayingMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack Failed"), AttackFailedAnims[0]);
				CurrentPlayingMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
				CurrentPlayingMontageTask->ReadyForActivation();

				SetComboAvailable(false);
				break;
			}
		case EWandererAttackResult::Miss:
			check(false);
		}

		InstigatorASC->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Trace);
		
		if(!InstigatorASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
		{
			Instigator->GetCombatComponent()->StartCombat();
		}
	}
	
}

bool UWandererActiveGameplayAbility_Attack::ShouldStopWeaponTrace()
{
	return !GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Trace);
}

EWandererAttackResult UWandererActiveGameplayAbility_Attack::EvaluateAttackResult(AWandererBaseCharacter* Target)
{
	if(Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Parry))
	{
		// Attack succeeds if from behind, even if target is parrying
		const FVector TargetForward = Target->GetActorForwardVector();
		const FVector AttackOrientation = GetAvatarActorFromActorInfo()->GetActorForwardVector();
		const float Cos = FVector::DotProduct(TargetForward, AttackOrientation);

		if(Cos < 0.7f) return EWandererAttackResult::Block;
	}
	/*if(Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Dodge))
	{
		return EWandererAttackResult::Miss;
	}*/
	
	
	return EWandererAttackResult::Success;
}

void UWandererActiveGameplayAbility_Attack::SetComboAvailable(bool bIsAvailable)
{
	if(bIsAvailable) GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
	else GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
}

bool UWandererActiveGameplayAbility_Attack::IsComboAvailable() const
{
	return GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(WandererGameplayTags::State_Attack_ComboAvailable);
}



