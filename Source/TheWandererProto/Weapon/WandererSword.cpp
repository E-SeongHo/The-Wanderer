//  


#include "Weapon/WandererSword.h"

#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Components/ArrowComponent.h"
#include "Utility/WandererUtils.h"
#include "Character/WandererBaseCharacter.h"

AWandererSword::AWandererSword() 
{
	EquippedTag = WandererGameplayTags::State_Equip_Sword;
	DrawnTag = WandererGameplayTags::State_Draw_Sword;
	
	PrimaryActorTick.bCanEverTick = false;
	SwordMesh = EquipmentMesh.Get();
	
	SheathMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sheath"));
	SheathMesh->SetupAttachment(RootComponent);
	
	SheathMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BladeStart = CreateDefaultSubobject<UArrowComponent>(TEXT("Blade Start"));
	BladeEnd = CreateDefaultSubobject<UArrowComponent>(TEXT("Blade End"));

	BladeStart->SetupAttachment(SwordMesh);
	BladeEnd->SetupAttachment(SwordMesh);
}

bool AWandererSword::Trace(FHitResult& OutHit)
{
	FCollisionQueryParams Param;
	Param.AddIgnoredActor(EquipmentOwner);
	const bool bHit = WandererUtils::SphereTrace(OutHit, this, BladeStart->GetComponentLocation(), BladeEnd->GetComponentLocation(), 5.0f, ECC_GameTraceChannel1, Param);
	
	return bHit;
}
