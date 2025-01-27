// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace WandererGameplayTags
{
	// Declare all of the custom native tags
	// Can get the gameplay tag without requesting function call

	// Input Tags
	// Natives
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ModifyAttack);

	// Abilities
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Vault);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Parry);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_TargetLock);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleWeapon1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleWeapon2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleWeapon3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleWeapon4);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Walk);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Dodge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Roll);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Finisher);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_TargetLock);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DrawWeapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Dodge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Walk);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Die);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Parry);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_KnockBack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_AutoTarget);

	// Cooldowns
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Parry);
	
	// AnimNotifies (including notify state)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_DrawSword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_SheathSword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_ComboAvailable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Charge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_WeaponTrace);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_Land);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_VictimDown);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_ParrySucceeded);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_ParryFailed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_KnockBack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_KnockBack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_Finisher);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_Victim);

	// GameplayCues
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Effect_BloodSplatter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Sound_Flesh);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Sound_Parry);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Sound_WeaponTrace);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Sound_Attack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Sound_Hit)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Sound_Die)
	
	// States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Equip);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Equip_Sword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Equip_Shield);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Draw);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Draw_Sword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Draw_Shield);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Weapon_Trace);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Charge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Stance_RightLead);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_TargetLock);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Parry);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attack_ComboAvailable);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Avoid);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_CanRecovery);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_KnockBack);
	
	// Damages
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage_Base);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage_Critical);

	// Action Tags for matching montages
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_RightLead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_LeftLead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_Counter);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_Counter_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_Counter_Left);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Attack_StrongAttack);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_AttackFailed);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Pair_Finisher);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Pair_Finisher_Front);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Pair_Finisher_Back);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_DrawWeapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_SheathWeapon);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Parry);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Parry_Front);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Parry_Back);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Parry_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Parry_Left);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Parry_Rebound);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Hit_Front);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Hit_Back);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Hit_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Hit_Left);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Die);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Die_Front);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Die_Back);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Die_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Die_Left);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Dodge);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Dodge_Front);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Dodge_Back);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Dodge_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Dodge_Left);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Roll);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Roll_Front);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Roll_Back);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Roll_Right);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActionTag_Roll_Left);

	// For Debugging
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debug);
};