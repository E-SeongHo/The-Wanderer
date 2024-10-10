//  


#include "Character/Component/WandererEquipmentComponent.h"

UWandererEquipmentComponent::UWandererEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TTuple<EWandererEquipmentSlot, AWandererWeapon*> UWandererEquipmentComponent::GetCurrentWeaponInfo() const
{
	if(Equipments.IsEmpty()) return TTuple<EWandererEquipmentSlot, AWandererWeapon*>();
	
	if(Equipments.Find(EWandererEquipmentSlot::Weapon1) && Equipments[EWandererEquipmentSlot::Weapon1]->IsDrawn())
	{
		return {EWandererEquipmentSlot::Weapon1, CastChecked<AWandererWeapon>(Equipments[EWandererEquipmentSlot::Weapon1])};
	}
	if(Equipments.Find(EWandererEquipmentSlot::Weapon2) && Equipments[EWandererEquipmentSlot::Weapon2]->IsDrawn())
	{
		return {EWandererEquipmentSlot::Weapon2, CastChecked<AWandererWeapon>(Equipments[EWandererEquipmentSlot::Weapon2])};
	}
	if(Equipments.Find(EWandererEquipmentSlot::Weapon3) && Equipments[EWandererEquipmentSlot::Weapon3]->IsDrawn())
	{
		return {EWandererEquipmentSlot::Weapon3, CastChecked<AWandererWeapon>(Equipments[EWandererEquipmentSlot::Weapon3])};
	}
	if(Equipments.Find(EWandererEquipmentSlot::Weapon4) && Equipments[EWandererEquipmentSlot::Weapon4]->IsDrawn())
    {
    	return {EWandererEquipmentSlot::Weapon4, CastChecked<AWandererWeapon>(Equipments[EWandererEquipmentSlot::Weapon4])};
    }
	
	return TTuple<EWandererEquipmentSlot, AWandererWeapon*>();
}

AWandererEquipment* UWandererEquipmentComponent::GetEquipmentOnSlot(const EWandererEquipmentSlot Slot) const
{
	if(!Equipments.Find(Slot)) return nullptr;

	return Equipments[Slot];
}

void UWandererEquipmentComponent::DrawEquipmentOnSlot(const EWandererEquipmentSlot Slot) const
{
	AWandererEquipment* Equipment = GetEquipmentOnSlot(Slot);
	check(Equipment);

	Equipment->OnDraw();
}

void UWandererEquipmentComponent::SheathEquipmentOnSlot(const EWandererEquipmentSlot Slot) const
{
	AWandererEquipment* Equipment = GetEquipmentOnSlot(Slot);
	check(Equipment);

	Equipment->OnSheath();
}

void UWandererEquipmentComponent::DropAllDrawnEquipments()
{
	for(const auto [Slot, Equipment] : Equipments)
	{
		if(Equipment->IsDrawn())
		{
			Equipment->OnDropDown();
		}
	}
}

void UWandererEquipmentComponent::AssignAbilitySystemComponent(UAbilitySystemComponent* OwnerASC)
{
	Super::AssignAbilitySystemComponent(OwnerASC);

	for(const auto& [Slot, Type] : StartUpEquipmentsConfig.EquipmentTypes)
	{
		if(Type)
		{
			AWandererEquipment* Equipment = GetWorld()->SpawnActor<AWandererEquipment>(Type, FVector::ZeroVector, FRotator::ZeroRotator);
			Equipments.Add(Slot, Equipment);
			Equipments[Slot]->InitializeWithOwner(Owner.Get());	
		}
	}
}
