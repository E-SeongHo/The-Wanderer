//  


#include "Weapon/WandererShield.h"

#include "WandererGameplayTags.h"

AWandererShield::AWandererShield()
{
	EquippedTag = WandererGameplayTags::State_Equip_Shield;
	DrawnTag = WandererGameplayTags::State_Draw_Shield;

	ShieldMesh = EquipmentMesh.Get();
}
