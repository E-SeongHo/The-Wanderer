//  


#include "Character/Component/WandererEquipmentComponent.h"

UWandererEquipmentComponent::UWandererEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AWandererWeapon* UWandererEquipmentComponent::GetCurrentWeapon() const
{
	if(Equipments[EWandererEquipmentSlot::PrimaryWeapon]->IsDrawn())
	{
		return CastChecked<AWandererWeapon>(Equipments[EWandererEquipmentSlot::PrimaryWeapon]);
	}
	if(Equipments[EWandererEquipmentSlot::SecondaryWeapon]->IsDrawn())
	{
		return CastChecked<AWandererWeapon>(Equipments[EWandererEquipmentSlot::SecondaryWeapon]);
	}	
	return nullptr;
}

AWandererEquipment* UWandererEquipmentComponent::GetEquipmentOnSlot(EWandererEquipmentSlot Slot) const
{
	if(!Equipments.Find(Slot)) return nullptr;

	return Equipments[Slot];	
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
