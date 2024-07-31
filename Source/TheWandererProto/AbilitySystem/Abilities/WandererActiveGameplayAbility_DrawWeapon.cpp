//  


#include "WandererActiveGameplayAbility_DrawWeapon.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/WandererBaseCharacter.h"
#include "Character/WandererCombatComponent.h"

UWandererActiveGameplayAbility_DrawWeapon::UWandererActiveGameplayAbility_DrawWeapon()
	: Super(WandererGameplayTags::InputTag_ToggleWeapon_Sword)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(WandererGameplayTags::Ability_DrawWeapon);
	// TODO: it temporarily indicates sword. would be an abstract base class for other weapons
	ActivationRequiredTags.AddTag(WandererGameplayTags::State_Equip_Sword);

	// ActivationOwnedTags.AddTag(WandererGameplayTags::State_Draw_Sword);

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = WandererGameplayTags::State_Combat;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::OwnedTagAdded; // Ending Ability needs sheathing
	AbilityTriggers.Add(TriggerData);
}

void UWandererActiveGameplayAbility_DrawWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// This ability is ended with sheath animation
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	UAbilitySystemComponent* ASC = Instigator->GetAbilitySystemComponent();
	
	UAbilityTask_PlayMontageAndWait* DrawSwordAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DrawSword"), DrawAnim);
	DrawSwordAnimTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGrabInHand = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_DrawSword);
	WaitGrabInHand->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::OnDraw);
	WaitGrabInHand->ReadyForActivation();
	
	Instigator->GetCombatComponent()->OnCombatEnded.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::SheathAndEndAbility);
}

void UWandererActiveGameplayAbility_DrawWeapon::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	check(IsActive());
	SheathAndEndAbility();
}

void UWandererActiveGameplayAbility_DrawWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(ActorInfo->AvatarActor);
	Instigator->GetCombatComponent()->OnCombatEnded.RemoveDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::SheathAndEndAbility);

	// If still in combat, sheathing the sword implies an intent to runaway
	if(Instigator->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat))
	{
		Instigator->GetCombatComponent()->Runaway();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_DrawWeapon::OnSheathCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_DrawWeapon::OnDraw(FGameplayEventData Payload)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetActorInfo().AvatarActor);
	Instigator->GetCombatComponent()->AttachWeaponToDrawSocket();
	Instigator->GetAbilitySystemComponent()->AddLooseGameplayTag(WandererGameplayTags::State_Draw_Sword);
	// TODO: infinite GE for Weapon's attributes
}

void UWandererActiveGameplayAbility_DrawWeapon::SheathAndEndAbility()
{
	UAbilityTask_PlayMontageAndWait* SheathSwordAnimTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SheathSword"), SheathAnim);
	SheathSwordAnimTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::OnSheathCompleted);
	SheathSwordAnimTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitReleaseInHand = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_SheathSword);
	WaitReleaseInHand->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_DrawWeapon::OnSheath);
	WaitReleaseInHand->ReadyForActivation();
}

void UWandererActiveGameplayAbility_DrawWeapon::OnSheath(FGameplayEventData Payload)
{
	AWandererBaseCharacter* Instigator = Cast<AWandererBaseCharacter>(GetActorInfo().AvatarActor);
	Instigator->GetCombatComponent()->AttachWeaponToSheathSocket();
	Instigator->GetAbilitySystemComponent()->RemoveLooseGameplayTag(WandererGameplayTags::State_Draw_Sword);
}
