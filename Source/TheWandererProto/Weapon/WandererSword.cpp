//  


#include "Weapon/WandererSword.h"

#include "WandererGameplayTags.h"
#include "Components/ArrowComponent.h"
#include "Utility/WandererUtils.h"

AWandererSword::AWandererSword() 
{
	EquippedTag = WandererGameplayTags::State_Equip_Sword;
	DrawnTag = WandererGameplayTags::State_Draw_Sword;
	
	PrimaryActorTick.bCanEverTick = true;
	SwordMesh = WeaponMesh;
	
	SheathMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sheath"));
	SheathMesh->SetupAttachment(RootComponent);
	
	// hack
	SheathMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BladeStart = CreateDefaultSubobject<UArrowComponent>(TEXT("Blade Start"));
	BladeEnd = CreateDefaultSubobject<UArrowComponent>(TEXT("Blade End"));

	BladeStart->SetupAttachment(SwordMesh);
	BladeEnd->SetupAttachment(SwordMesh);
}

bool AWandererSword::Trace(FHitResult& OutHit)
{
	const bool bHit = WandererUtils::SphereTrace(OutHit, this, BladeStart->GetComponentLocation(), BladeEnd->GetComponentLocation(), 5.0f, ECC_GameTraceChannel1);
	
	return bHit;
}
