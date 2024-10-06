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
	return EquipmentOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(DrawnTag);
}

void AWandererEquipment::InitializeWithOwner(AWandererBaseCharacter* InOwner)
{
	check(!EquipmentOwner);
    
	EquipmentOwner = InOwner;
	EquipmentOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(EquippedTag);

	AttachToComponent(EquipmentOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipSocket);
}

void AWandererEquipment::OnDraw()
{
	check(EquipmentOwner);
	EquipmentOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(DrawnTag);
	EquipmentMesh->AttachToComponent(EquipmentOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, DrawSocket);
}

void AWandererEquipment::OnSheath()
{
	check(EquipmentOwner);
	EquipmentOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(DrawnTag);
	EquipmentMesh->AttachToComponent(EquipmentOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipSocket);
}

void AWandererEquipment::OnDropDown()
{
	EquipmentOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(EquippedTag);
	EquipmentOwner->GetAbilitySystemComponent()->RemoveLooseGameplayTag(DrawnTag);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquipmentMesh->SetSimulatePhysics(true);
}
