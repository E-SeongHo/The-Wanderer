//  


#include "Weapon/WandererWeapon.h"

#include "Character/WandererBaseCharacter.h"
#include "Character/Component/WandererEquipmentComponent.h"

AWandererWeapon::AWandererWeapon()
{
}

void AWandererWeapon::OnDraw()
{
	Super::OnDraw();
	if(HandType == EWandererWeaponHandType::OneHand)
	{
		AWandererEquipment* MergableShield = EquipmentOwner->FindComponentByClass<UWandererEquipmentComponent>()->GetEquipmentOnSlot(EWandererEquipmentSlot::Shield);
		if(MergableShield)
		{
			MergableShield->OnDraw();
		}
	}
}

void AWandererWeapon::OnSheath()
{
	Super::OnSheath();
	if(HandType == EWandererWeaponHandType::OneHand)
	{
		AWandererEquipment* MergableShield = EquipmentOwner->FindComponentByClass<UWandererEquipmentComponent>()->GetEquipmentOnSlot(EWandererEquipmentSlot::Shield);
		if(MergableShield)
		{
			MergableShield->OnSheath();
		}
	}
}
