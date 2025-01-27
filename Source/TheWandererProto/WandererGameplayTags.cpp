// Fill out your copyright notice in the Description page of Project Settings.

#include "WandererGameplayTags.h"

namespace WandererGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ModifyAttack, "InputTag.ModifyAttack", "Modify attack input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Vault, "InputTag.Vault", "Vault input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Attack, "InputTag.Attack", "Attack input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_StrongAttack, "InputTag.StrongAttack", "Attack input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Parry, "InputTag.Parry", "Parry input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_TargetLock, "InputTag.TargetLock", "Target lock input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWeapon1, "InputTag.ToggleWeapon1", "Toggle weapon on slot 1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWeapon2, "InputTag.ToggleWeapon2", "Toggle weapon on slot 2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWeapon3, "InputTag.ToggleWeapon3", "Toggle weapon on slot 3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWeapon4, "InputTag.ToggleWeapon4", "Toggle weapon on slot 4");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Sprint, "InputTag.Sprint", "Sprint input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Walk, "InputTag.Walk", "Walk input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Dodge, "InputTag.Dodge", "Dodge input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Roll, "InputTag.Roll", "Roll input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack, "Ability.Attack", "Attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Finisher, "Ability.Finisher", "Finisher");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Sprint, "Ability.Sprint", "Sprint");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_TargetLock, "Ability.TargetLock", "Target lock");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_DrawWeapon, "Ability.DrawWeapon", "Draw weapon");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Dodge, "Ability.Dodge", "Dodge");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Roll, "Ability.Roll", "Roll");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Walk, "Ability.Walk", "Walk");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Die, "Ability.Die", "Die");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Hit, "Ability.Hit", "Hit reaction when get hit");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Parry, "Ability.Parry", "Parry attack when get hit");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_KnockBack, "Ability.KnockBack", "Knockback reaction");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_AutoTarget, "Ability.AutoTarget", "Auto Lock Target (this is not same with target lock(hard lock)");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Parry, "Cooldown.Parry", "Parry cooldown effect tag");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_DrawSword, "Event.Montage.DrawSword", "Draw sword animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_SheathSword, "Event.Montage.SheathSword", "Sheath sword animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_ComboAvailable, "Event.Montage.ComboAvailable", "ComboAvailable animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_WeaponTrace, "Event.Montage.WeaponTrace", "WeaponTrace animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_Land, "Event.Montage.Land", "Landed again to the ground animation event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_Charge, "Event.Montage.Charge", "Strong attack charge entry point event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_VictimDown, "Event.Montage.VictimDown", "The end point of the victim animation");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Combat_ParrySucceeded, "Event.Combat.ParrySucceeded", "Succeeded to parry attack event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Combat_ParryFailed, "Event.Combat.ParryFailed", "Failed to parry attack event");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Combat_KnockBack, "Event.Combat.KnockBack", "Knockback event, this can be occured by various situations");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Combat_Finisher, "Event.Combat.Finisher", "Finisher event to activate finisher ability with event data");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Combat_Victim, "Event.Combat.Victim", "Victim event to activate die ability with event data");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Effect_BloodSplatter, "GameplayCue.Effect.BloodSplatter", "Blood splatter gameplay cue");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Sound_Flesh, "GameplayCue.Sound.Flesh", "Flesh sound gameplay cue");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Sound_Parry, "GameplayCue.Sound.Parry", "Parry sound gameplay cue");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Sound_WeaponTrace, "GameplayCue.Sound.WeaponTrace", "Sword trace sound gameplay cue");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Sound_Attack, "GameplayCue.Sound.Attack", "Attack sound gameplay cue");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Sound_Hit, "GameplayCue.Sound.Hit", "Hit sound gameplay cue");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Sound_Die, "GameplayCue.Sound.Die", "Die sound gameplay cue");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead", "Dead status");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Equip, "State.Equip", "Tags for equipped statements, it exists for making query");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Equip_Sword, "State.Equip.Sword", "Tags for equipped statements");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Equip_Shield, "State.Equip.Shield", "Tags for equipped statements");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Draw, "State.Draw", "Tags for drawn statements, it exists for making query");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Draw_Sword, "State.Draw.Sword", "Tags for drawn statements");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Draw_Shield, "State.Draw.Shield", "Tags for drawn statements");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Weapon_Trace, "State.Weapon.Trace", "Weapon tracing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Charge, "State.Charge", "Charging attack");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Stance_RightLead, "State.Stance.RightLead", "Right lead stance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Parry, "State.Parry", "On parry");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Combat, "State.Combat", "On combat");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Combat_TargetLock, "State.Combat.TargetLock", "Target locking");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Attack_ComboAvailable, "State.Attack.ComboAvailable", "Combo available state while attack ability is working on");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Avoid, "State.Avoid", "Avoiding. not get damage");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_CanRecovery, "State.CanRecovery", "Can recovery while hitting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_KnockBack, "State.KnockBack", "Knockbacking while after parry succeeded");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage_Base, "Data.Damage.Base", "Base damage data");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage_Critical, "Data.Damage.Critical", "Critical damage data");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack, "ActionTag.Attack", "Attack action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_RightLead, "ActionTag.Attack.RightLead", "Attack action from rightlead status");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_LeftLead, "ActionTag.Attack.LeftLead", "Attack action from leftlead status");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_Counter, "ActionTag.Attack.Counter", "Counter attack action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_Counter_Right, "ActionTag.Attack.Counter.Right", "Avoid to the right and counter");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_Counter_Left, "ActionTag.Attack.Counter.Left", "Avoid to the left and counter");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_Dash, "ActionTag.Attack.Dash", "Dash attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Attack_StrongAttack, "ActionTag.Attack.StrongAttack", "Strong attack");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_AttackFailed, "ActionTag.AttackFailed", "Attack failed action");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Pair_Finisher, "ActionTag.Pair.Finisher", "Finisher pair action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Pair_Finisher_Front, "ActionTag.Pair.Finisher.Front", "Finisher from forward direction pair action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Pair_Finisher_Back, "ActionTag.Pair.Finisher.Back", "Finisher from backward direction(implies assasinate) pair action");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_DrawWeapon, "ActionTag.DrawWeapon", "Draw weapon action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_SheathWeapon, "ActionTag.SheathWeapon", "Sheath weapon action");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Parry, "ActionTag.Parry", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Parry_Front, "ActionTag.Parry.Front", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Parry_Back, "ActionTag.Parry.Back", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Parry_Right, "ActionTag.Parry.Right", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Parry_Left, "ActionTag.Parry.Left", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Parry_Rebound, "ActionTag.Parry.Rebound", "Rebound when parry failed");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Hit, "ActionTag.Hit", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Hit_Front, "ActionTag.Hit.Front", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Hit_Back, "ActionTag.Hit.Back", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Hit_Right, "ActionTag.Hit.Right", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Hit_Left, "ActionTag.Hit.Left", "ActionTag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Die, "ActionTag.Die", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Die_Front, "ActionTag.Die.Front", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Die_Back, "ActionTag.Die.Back", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Die_Right, "ActionTag.Die.Right", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Die_Left, "ActionTag.Die.Left", "ActionTag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Dodge, "ActionTag.Dodge", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Dodge_Front, "ActionTag.Dodge.Front", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Dodge_Back, "ActionTag.Dodge.Back", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Dodge_Right, "ActionTag.Dodge.Right", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Dodge_Left, "ActionTag.Dodge.Left", "ActionTag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Roll, "ActionTag.Roll", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Roll_Front, "ActionTag.Roll.Front", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Roll_Back, "ActionTag.Roll.Back", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Roll_Right, "ActionTag.Roll.Right", "ActionTag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActionTag_Roll_Left, "ActionTag.Roll.Left", "ActionTag");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debug, "Debug", "Activation Tag for debugging");
};