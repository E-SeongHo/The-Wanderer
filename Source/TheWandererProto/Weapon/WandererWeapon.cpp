//  


#include "Weapon/WandererWeapon.h"

#include "AbilitySystemComponent.h"
#include "Character/WandererBaseCharacter.h"

// Sets default values
AWandererWeapon::AWandererWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWandererWeapon::InitializeWithOwner(AWandererBaseCharacter* InWeaponOwner)
{
	check(!WeaponOwner);
    
	WeaponOwner = InWeaponOwner;
	WeaponOwner->GetAbilitySystemComponent()->AddLooseGameplayTag(EquippedTag);
}




