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

	// Abilities
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Vault);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Parry);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_TargetLock);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleWeapon_Sword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Walk);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Sprint);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_TargetLock);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_DrawWeapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Walk);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Die);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Guard);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Avoid);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_AutoTarget);
	
	// AnimNotifies (including notify state)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_DrawSword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_SheathSword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_ComboAvailable);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_WeaponTrace);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Combat_ParryAttack);
	
	// States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Equip);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Equip_Sword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Draw);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Draw_Sword);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Weapon_Trace);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Stance_RightLead);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_TargetLock);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Parry);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Attack_ComboAvailable);
	
	// Damages
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage_Base);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage_Critical);

	// For Debugging
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debug);
};