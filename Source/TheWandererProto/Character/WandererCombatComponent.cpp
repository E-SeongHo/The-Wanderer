//  


#include "Character/WandererCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "WandererEnemy.h"
#include "WandererGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WandererWeapon.h"

UWandererCombatComponent::UWandererCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWandererCombatComponent::AssignAbilitySystemComponent(UAbilitySystemComponent* OwnerASC)
{
	Super::AssignAbilitySystemComponent(OwnerASC);
}

void UWandererCombatComponent::EquipWeapon(AWandererWeapon* NewWeapon, FName SocketName)
{
	Weapon = NewWeapon;
	Weapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	AbilitySystemComponent->AddLooseGameplayTag(Weapon->EquippedTag);	
}

void UWandererCombatComponent::AttachWeaponMeshToSocket(FName SocketName)
{
	check(Weapon);
	Weapon->GetWeaponMesh()->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

void UWandererCombatComponent::SetCombatTarget(AWandererBaseCharacter* InTarget)
{
	if(InTarget == CombatTarget) return;

	// Combat can be started via
	// 1) Set valid Combat Target
	// 2) Directly call StartCombat()
	if(!bIsInCombat)
	{
		StartCombat();	
	}
	
	if(Cast<AWandererEnemy>(CombatTarget))
	{
		Cast<AWandererEnemy>(CombatTarget)->SetUIRender(false);
	}
	if(Cast<AWandererEnemy>(InTarget))
	{
		Cast<AWandererEnemy>(InTarget)->SetUIRender(true);
	}
	
	if(!InTarget)
	{
		FTimerHandle CombatExitDelayHandle;
		GetWorld()->GetTimerManager().SetTimer(CombatExitDelayHandle, [this]
		{
			if(!CombatTarget)
			{
				EndCombat();
			}
		}, CombatExitDelay, false);
	}

	CombatTarget = InTarget;
}

void UWandererCombatComponent::StartCombat()
{
	check(!bIsInCombat);
	// bIsInCombat can be true whether the CombatTarget set or not
	// but when its becoming false CombatTarget will be nullptr
	bIsInCombat = true;  
	
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	
	AbilitySystemComponent->AddLooseGameplayTag(WandererGameplayTags::State_Combat);
}

void UWandererCombatComponent::EndCombat()
{
	check(bIsInCombat);
	check(!CombatTarget);
	bIsInCombat = false;

	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;

	AbilitySystemComponent->RemoveLooseGameplayTag(WandererGameplayTags::State_Combat);
}

void UWandererCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!AbilitySystemComponent.Get()) return;
	if(!bIsInCombat) return;
	if(!CombatTarget) return;
	
	// Rotates the actor itself to face the target
	const FVector Forward = Owner->GetActorForwardVector();
	const FVector ToTarget = (CombatTarget->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal2D();
	Owner->SetActorRotation(FMath::RInterpTo(Forward.Rotation(), ToTarget.Rotation(), DeltaTime, 5.0f));

	// in order to rotate cameraboom (make sure this controlled by controller)
	if(bUseActorDesiredControlRotation)
	{
		Owner->GetController()->SetControlRotation(FMath::RInterpTo(Owner->GetControlRotation(), ToTarget.Rotation(), DeltaTime, 5.0f));
	}
}


