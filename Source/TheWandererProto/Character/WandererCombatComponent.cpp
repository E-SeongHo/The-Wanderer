//  


#include "Character/WandererCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "Enemy/WandererEnemy.h"
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

	EquipWeapon();
}

bool UWandererCombatComponent::IsTargetInAttackRange() const
{
	if(!CombatTarget) return false;
	
	return Owner->GetDistanceTo(CombatTarget) < AttackAvailableDistance;
}

void UWandererCombatComponent::AttachWeaponToDrawSocket() const
{
	check(Weapon);
	Weapon->GetWeaponMesh()->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponConfig.DrawSocket);
}

void UWandererCombatComponent::AttachWeaponToSheathSocket() const
{
	check(Weapon);
	Weapon->GetWeaponMesh()->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponConfig.SheathSocket);
}

void UWandererCombatComponent::SetCombatTarget(AWandererBaseCharacter* InTarget)
{
	if(InTarget == CombatTarget) return;

	// Combat can be started via
	// 1) Set valid Combat Target
	// 2) Directly call StartCombat()
	if(!bIsInCombat)
	{
		check(!CombatTarget);
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
			if(!CombatTarget && bIsInCombat) // Combat may have already ended due to a runaway
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
	OnCombatStarted.Broadcast();
	
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	
	AbilitySystemComponent->AddLooseGameplayTag(WandererGameplayTags::State_Combat);
}

void UWandererCombatComponent::EndCombat()
{
	check(bIsInCombat);
	check(!CombatTarget);
	bIsInCombat = false;
	OnCombatEnded.Broadcast();

	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;

	AbilitySystemComponent->RemoveLooseGameplayTag(WandererGameplayTags::State_Combat);
}

void UWandererCombatComponent::Runaway()
{
	check(bIsInCombat);

	CombatTarget = nullptr;

	// End combat immediately
	EndCombat();
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
	if(FVector::DotProduct(Forward, ToTarget) < 0.999f)
	{
		Owner->SetActorRotation(FMath::RInterpTo(Forward.Rotation(), ToTarget.Rotation(), DeltaTime, 5.0f));
	}

	// in order to rotate cameraboom (make sure this controlled by controller)
	if(bUseActorDesiredControlRotation)
	{
		Owner->GetController()->SetControlRotation(FMath::RInterpTo(Owner->GetControlRotation(), ToTarget.Rotation(), DeltaTime, 5.0f));
	}
}

void UWandererCombatComponent::EquipWeapon()
{
	if(WeaponConfig.WeaponType)
	{
		Weapon = GetWorld()->SpawnActor<AWandererWeapon>(WeaponConfig.WeaponType, FVector::ZeroVector, FRotator::ZeroRotator);
		Weapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponConfig.SheathSocket);
		AbilitySystemComponent->AddLooseGameplayTag(Weapon->EquippedTag);	
	}
}


