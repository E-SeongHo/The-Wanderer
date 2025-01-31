//  


#include "WandererActiveGameplayAbility_DrawWeapon.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Animation/WandererAnimInstance.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/Component/WandererCombatComponent.h"

UWandererActiveGameplayAbility_DrawWeapon::UWandererActiveGameplayAbility_DrawWeapon()
	: Super(WandererGameplayTags::InputTag_ToggleWeapon1)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_DrawWeapon);
	ActivationOwnedTags.AddTag(WandererGameplayTags::Ability_DrawWeapon);
	ActivationBlockedTags.AddTag(WandererGameplayTags::State_Draw);
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = WandererGameplayTags::State_Combat;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded; // Ending Ability needs sheathing
	AbilityTriggers.Add(TriggerData);
}

void UWandererActiveGameplayAbility_DrawWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// only primary weapon will be drawn when this ability is activated by starting combat 
	if(TriggerEventData && TriggerEventData->EventTag == WandererGameplayTags::State_Combat)
	{
		if(WeaponSlot != EWandererEquipmentSlot::Weapon1)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Draw from combat starting")));
	}
	
	// This ability is ended with sheath animation
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	UAbilitySystemComponent* InstigatorASC = Instigator->GetAbilitySystemComponent();
	
	CastChecked<UWandererAnimInstance>(ActorInfo->GetAnimInstance())->OnActiveWeaponSlotChanged(WeaponSlot);
	
	UAbilityTask_PlayMontageAndWait* DrawWeaponAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DrawWeapon"), GetMatchingMontageForTag(WandererGameplayTags::ActionTag_DrawWeapon));
	DrawWeaponAnimTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGrabInHand = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_DrawSword);
	WaitGrabInHand->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::OnDraw);
	WaitGrabInHand->ReadyForActivation();

	if(InstigatorASC->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		CreateWaitCombatEndTask();
	}
	else
	{
		UAbilityTask_WaitGameplayTagAdded* WaitCombatStart = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, WandererGameplayTags::State_Combat);
		WaitCombatStart->Added.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::CreateWaitCombatEndTask);
		WaitCombatStart->ReadyForActivation();
	}
	
	bIsSheathing = false;
}

void UWandererActiveGameplayAbility_DrawWeapon::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	check(IsActive());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DrawWeapon Input")));

	if(DoesOwnerHaveTag(WandererGameplayTags::State_Draw) && !bIsSheathing)
	{
		SheathAndEndAbility();
	}
}

void UWandererActiveGameplayAbility_DrawWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	
	// If still in combat, sheathing the sword implies an intent to runaway
	if(!bWasCancelled && Instigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		Instigator->GetCombatComponent()->Runaway();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_DrawWeapon::OnSheathCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_DrawWeapon::CreateWaitCombatEndTask()
{
	check(DoesOwnerHaveTag(WandererGameplayTags::State_Combat));
	UAbilityTask_WaitGameplayTagRemoved* WaitCombatEnd = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, WandererGameplayTags::State_Combat);
	WaitCombatEnd->Removed.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::SheathAndEndAbility);
	WaitCombatEnd->ReadyForActivation();
}

void UWandererActiveGameplayAbility_DrawWeapon::OnDraw(FGameplayEventData Payload)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetActorInfo().AvatarActor);
	Instigator->FindComponentByClass<UWandererEquipmentComponent>()->DrawEquipmentOnSlot(WeaponSlot);
}

void UWandererActiveGameplayAbility_DrawWeapon::SheathAndEndAbility()
{
	if(bIsSheathing) return;
	bIsSheathing = true;
	
	UAbilityTask_PlayMontageAndWait* SheathSwordAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SheathSword"), GetMatchingMontageForTag(WandererGameplayTags::ActionTag_SheathWeapon));
	SheathSwordAnimTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::OnSheathCompleted);
	SheathSwordAnimTask->ReadyForActivation();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Sheath and End")));
	
	UAbilityTask_WaitGameplayEvent* WaitReleaseInHand = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_SheathSword);
	WaitReleaseInHand->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::OnSheath);
	WaitReleaseInHand->ReadyForActivation();
}

void UWandererActiveGameplayAbility_DrawWeapon::OnSheath(FGameplayEventData Payload)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetActorInfo().AvatarActor);
	Instigator->FindComponentByClass<UWandererEquipmentComponent>()->SheathEquipmentOnSlot(WeaponSlot);
}
