//  


#include "WandererActiveGameplayAbility_ToggleWeapon.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/WandererCharacter.h"

UWandererActiveGameplayAbility_ToggleWeapon::UWandererActiveGameplayAbility_ToggleWeapon()
	: Super(WandererGameplayTags::InputTag_ToggleWeapon_Sword)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UWandererActiveGameplayAbility_ToggleWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AWandererCharacter* WandererCharacter = Cast<AWandererCharacter>(ActorInfo->AvatarActor.Get());
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Draw Sword!")));	

	UAbilitySystemComponent* ASC = WandererCharacter->GetAbilitySystemComponent();
	if(ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Unarmed))
	{
		ASC->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Unarmed);
		ASC->AddLooseGameplayTag(WandererGameplayTags::State_Weapon_Sword);

		// Generate AbilityTask : Play Montage
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DrawSword"), DrawSwordAnim);
		PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_ToggleWeapon::OnMontageCompleted);
		PlayMontageTask->ReadyForActivation();

		// Generate AbilityTask : Wait Event (Event.Montage.DrawSword)
		UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_DrawSword);
		WaitEventTask->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_ToggleWeapon::OnDrawSword);
		WaitEventTask->ReadyForActivation();

		
	}
	else if(ASC->HasMatchingGameplayTag(WandererGameplayTags::State_Weapon_Sword))
	{
		ASC->RemoveLooseGameplayTag(WandererGameplayTags::State_Weapon_Sword);
		ASC->AddLooseGameplayTag(WandererGameplayTags::State_Weapon_Unarmed);

		// Generate AbilityTask : Play Montage
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SheathSword"), SheathSwordAnim);
		PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_ToggleWeapon::OnMontageCompleted);
		PlayMontageTask->ReadyForActivation();

		// Generate AbilityTask : Wait Event (Event.Montage.SheathSword)
		UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, WandererGameplayTags::Event_Montage_SheathSword);
		WaitEventTask->EventReceived.AddDynamic(this, &UWandererActiveGameplayAbility_ToggleWeapon::OnSheathSword);
		WaitEventTask->ReadyForActivation();
	}
	else
	{
		check(false);		
	}
}

void UWandererActiveGameplayAbility_ToggleWeapon::OnMontageCompleted()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Montage ended")));	
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UWandererActiveGameplayAbility_ToggleWeapon::OnDrawSword(FGameplayEventData Payload)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Grab Sword Notify!")));	

	AWandererCharacter* WandererCharacter = Cast<AWandererCharacter>(GetActorInfo().AvatarActor.Get());
	WandererCharacter->AttachWeaponMeshToSocket(TEXT("SwordSocket"));
}

void UWandererActiveGameplayAbility_ToggleWeapon::OnSheathSword(FGameplayEventData Payload)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Sheath Sword Notify!")));	

	AWandererCharacter* WandererCharacter = Cast<AWandererCharacter>(GetActorInfo().AvatarActor.Get());
	WandererCharacter->AttachWeaponMeshToSocket(TEXT("SheathSocket"));
}