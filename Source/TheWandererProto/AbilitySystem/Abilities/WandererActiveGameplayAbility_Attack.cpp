//  


#include "WandererActiveGameplayAbility_Attack.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
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
}

bool UWandererActiveGameplayAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// if nothing on hand(drawn weapon)
	if(!ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Draw)) return false;
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UWandererActiveGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	SoftLock();
	
	// Generate AbilityTask : Play Montage
	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), AttackAnimsFromLeftLead[FMath::RandRange(0, AttackAnimsFromLeftLead.Num()-1)]); 
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
		if(CurrentActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Equip_Sword))
		{
			UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_WeaponTrace);
			WaitEventTask->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnWeaponTraceStart);
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
		PlayMontageTask->ExternalCancel();
		
		SoftLock();
		UAnimMontage* MontageToPlay;
		if(ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(WandererGameplayTags::State_Stance_RightLead))
		{
			MontageToPlay = AttackAnimsFromRightLead[FMath::RandRange(0, AttackAnimsFromRightLead.Num()-1)];
		}
		else
		{
			MontageToPlay = AttackAnimsFromLeftLead[FMath::RandRange(0, AttackAnimsFromLeftLead.Num()-1)];
		}
		
		PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Attack"), MontageToPlay);
		PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Attack::OnMontageCompleted);
		PlayMontageTask->ReadyForActivation();
		
		ComboCount++;
		bIsComboAvailable = false;
	}
}

void UWandererActiveGameplayAbility_Attack::SoftLock()
{
	AWandererCharacter* Instigator = Cast<AWandererCharacter>(GetCurrentActorInfo()->AvatarActor);
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
	bIsComboAvailable = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_Attack::OnComboAvailable(FGameplayEventData Payload)
{
	bIsComboAvailable = true;
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

	if(InstigatorASC->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Trace))
	{
		FHitResult HitResult;
		bool bHit = Instigator->GetCombatComponent()->GetWeapon()->Trace(HitResult);
		if(bHit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Cyan, false, 1.0f);

			const AWandererBaseCharacter* Target = Cast<AWandererBaseCharacter>(HitResult.GetActor());
			if(Target)
			{
				// 1. Damage Gameplay Effect
				check(DamageEffect);
				const FGameplayAbilityTargetDataHandle TargetHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
				const FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), InstigatorASC->MakeEffectContext());

				const AWandererBaseCharacter* CauserWandererCharacter = Cast<AWandererBaseCharacter>(this->GetActorInfo().AvatarActor);
				SpecHandle.Data->SetSetByCallerMagnitude(WandererGameplayTags::Data_Damage_Base, CauserWandererCharacter->GetCombatAttributeSet()->GetBaseDamage() * 0.25f);
					
				ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SpecHandle, TargetHandle);
				// ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetHandle, DamageEffect, GetAbilityLevel());

				// 2. Blood Effect TODO: Gameplay Cue
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, HitResult.ImpactPoint);
				
				InstigatorASC->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Trace);
			}
		}
	}
}

bool UWandererActiveGameplayAbility_Attack::ShouldStopWeaponTrace()
{
	return !GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Trace);
}


