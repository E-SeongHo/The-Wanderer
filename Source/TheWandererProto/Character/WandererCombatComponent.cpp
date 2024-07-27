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

void UWandererCombatComponent::SetCombatTarget(AWandererBaseCharacter* Target)
{
	if(Target == CombatTarget) return;

	// Combat can be started via
	// 1) Set valid Combat Target
	// 2) Directly call StartCombat()
	if(!bIsInCombat)
	{
		StartCombat();	
	}
	
	OnTargetChanged.Broadcast();

	if(Target)
	{
		if(Cast<AWandererEnemy>(Target))
		{
			Cast<AWandererEnemy>(Target)->SetUIRender(true);
		}
	}
	else
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

	CombatTarget = Target;
}

void UWandererCombatComponent::StartCombat()
{
	check(!bIsInCombat);
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
	
	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + Forward * 100.0f, FColor::Red, false, 1.0f);
	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + ToTarget * 100.0f, FColor::Green, false, 1.0f);
	Owner->SetActorRotation(FMath::RInterpTo(Forward.Rotation(), ToTarget.Rotation(), DeltaTime, 5.0f));

	// in order to rotate cameraboom (make sure this controlled by controller)
	if(bUseActorDesiredControlRotation)
	{
		Owner->GetController()->SetControlRotation(FMath::RInterpTo(Owner->GetControlRotation(), ToTarget.Rotation(), DeltaTime, 5.0f));
	}
}


