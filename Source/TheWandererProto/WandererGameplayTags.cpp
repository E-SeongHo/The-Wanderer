// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererGameplayTags.h"

namespace WandererGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Vault, "InputTag.Vault", "Vault input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Attack, "InputTag.Attack", "Attack input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Parry, "InputTag.Parry", "Parry input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_TargetLock, "InputTag.TargetLock", "Target lock input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWeapon_Sword, "InputTag.ToggleWeapon.Sword", "Toggle weapon : Sword.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Sprint, "InputTag.Sprint", "Sprint input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Walk, "InputTag.Walk", "Walk input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack, "Ability.Attack", "Attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Sprint, "Ability.Sprint", "Sprint");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_TargetLock, "Ability.TargetLock", "Target lock");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_DrawWeapon, "Ability.DrawWeapon", "Draw weapon");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Walk, "Ability.Walk", "Walk");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Die, "Ability.Die", "Die");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Hit, "Ability.Hit", "Hit reaction when get hit");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Guard, "Ability.Guard", "Guard attack when get hit");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Avoid, "Ability.Avoid", "Avoid attack when get hit");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_AutoTarget, "Ability.AutoTarget", "Auto Lock Target (this is not same with target lock(hard lock)");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_DrawSword, "Event.Montage.DrawSword", "Draw sword animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_SheathSword, "Event.Montage.SheathSword", "Sheath sword animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_ComboAvailable, "Event.Montage.ComboAvailable", "ComboAvailable animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_WeaponTrace, "Event.Montage.WeaponTrace", "WeaponTrace animation event");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Combat_ParryAttack, "Event.Combat.ParryAttack", "Succeeded to parry attack event");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Equip, "State.Equip", "Tags for equip statements, it exists for making query");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Equip_Sword, "State.Equip.Sword", "Armed sword");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Draw, "State.Draw", "Tags for drawn statements, it exists for making query");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Draw_Sword, "State.Draw.Sword", "Drawn sword");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Weapon_Trace, "State.Weapon.Trace", "Weapon tracing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Stance_RightLead, "State.Stance.RightLead", "Right lead stance");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Combat, "State.Combat", "On combat");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Combat_Parry, "State.Combat.Parry", "On parry");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Combat_TargetLock, "State.Combat.TargetLock", "Target locking");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage_Base, "Data.Damage.Base", "Base damage data");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage_Critical, "Data.Damage.Critical", "Critical damage data");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debug, "Debug", "Activation Tag for debugging");
};