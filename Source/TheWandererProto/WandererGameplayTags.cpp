// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererGameplayTags.h"

namespace WandererGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Vault, "InputTag.Vault", "Vault input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Attack, "InputTag.Attack", "Attack input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_TargetLock, "InputTag.TargetLock", "Target Lock");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWeapon_Sword, "InputTag.ToggleWeapon.Sword", "Toggle weapon : Sword.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Die, "Ability.Die", "Die.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Hit, "Ability.Hit", "Hit reaction when get hit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Guard, "Ability.Guard", "Guard attack when get hit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Avoid, "Ability.Avoid", "Avoid attack when get hit.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_DrawSword, "Event.Montage.DrawSword", "Draw sword animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_SheathSword, "Event.Montage.SheathSword", "Sheath sword animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_ComboAvailable, "Event.Montage.ComboAvailable", "ComboAvailable animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_WeaponTrace, "Event.Montage.WeaponTrace", "WeaponTrace animation event");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Weapon_Unarmed, "State.Weapon.Unarmed", "Unarmed");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Weapon_Sword, "State.Weapon.Sword", "Armed Sword");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Weapon_Bow, "State.Weapon.Bow", "Armed Bow");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Weapon_Trace, "State.Weapon.Trace", "Weapon Tracing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Combat_TargetLock, "State.Combat.TargetLock", "Target Locking");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage_Base, "Data.Damage.Base", "Base Damage Data");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage_Critical, "Data.Damage.Critical", "Critical Damage Data");
};