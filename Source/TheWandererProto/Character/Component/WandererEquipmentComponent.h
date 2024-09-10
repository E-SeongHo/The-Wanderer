//  

#pragma once

#include "CoreMinimal.h"
#include "WandererActorComponent.h"
#include "Weapon/WandererEquipment.h"
#include "Weapon/WandererShield.h"
#include "Weapon/WandererWeapon.h"
#include "WandererEquipmentComponent.generated.h"


UENUM()
enum class EWandererEquipmentSlot
{
	PrimaryWeapon,
	SecondaryWeapon,
	Shield,
	// Armors, etc...
};

USTRUCT()
struct FWandererStartUpEquipmentConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<EWandererEquipmentSlot, TSubclassOf<AWandererEquipment>> EquipmentTypes;
};

UCLASS()
class THEWANDERERPROTO_API UWandererEquipmentComponent : public UWandererActorComponent
{
	GENERATED_BODY()

public:	
	UWandererEquipmentComponent();

	AWandererWeapon* GetCurrentWeapon() const;
	AWandererEquipment* GetEquipmentOnSlot(EWandererEquipmentSlot Slot) const;

	virtual void AssignAbilitySystemComponent(UAbilitySystemComponent* OwnerASC) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	FWandererStartUpEquipmentConfig StartUpEquipmentsConfig;

	UPROPERTY(VisibleAnywhere, Category = Equipment, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EWandererEquipmentSlot, AWandererEquipment*> Equipments;
};
