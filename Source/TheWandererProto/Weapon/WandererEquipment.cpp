//  


#include "Weapon/WandererEquipment.h"

#include "AbilitySystemComponent.h"
#include "Character/WandererBaseCharacter.h"

AWandererEquipment::AWandererEquipment()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Equipment"));
	EquipmentMesh->SetupAttachment(RootComponent);
	EquipmentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AWandererEquipment::IsDrawn() const
{
	return bIsDrawn;
}

void AWandererEquipment::InitializeWithOwner(AWandererBaseCharacter* InOwner)
{
	check(!EquipmentOwner);
    
	EquipmentOwner = InOwner;

	AttachToComponent(EquipmentOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipSocket);
	EquipmentOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(EquippedTag);
	bIsEquipped = true;
}

void AWandererEquipment::OnDraw()
{
	check(EquipmentOwner);
	check(bIsEquipped && !bIsDrawn);
	EquipmentMesh->AttachToComponent(EquipmentOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, DrawSocket);
	EquipmentOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(DrawnTag);
	bIsDrawn = true;
}

void AWandererEquipment::OnSheath()
{
	check(EquipmentOwner);
	check(bIsEquipped && bIsDrawn);
	EquipmentMesh->AttachToComponent(EquipmentOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipSocket);
	EquipmentOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(DrawnTag);
	bIsDrawn = false;
}

void AWandererEquipment::OnDropDown()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquipmentMesh->SetSimulatePhysics(true);

	EquipmentOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(EquippedTag);
	EquipmentOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(DrawnTag);
	bIsEquipped = false;
	bIsDrawn = false;
}
