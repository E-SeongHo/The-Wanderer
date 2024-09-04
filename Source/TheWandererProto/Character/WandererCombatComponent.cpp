//  


#include "Character/WandererCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "WandererBaseCharacter.h"
#include "Enemy/WandererEnemy.h"
#include "WandererGameplayTags.h"
#include "AbilitySystem/Attributes/WandererCombatAttributeSet.h"
#include "AbilitySystem/Attributes/WandererHealthAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/WandererUtils.h"
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
	if(CombatTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Dead)) return false;

	UE_LOG(LogTemp, Display, TEXT("%f distanceo to attack"), Owner->GetDistanceTo(CombatTarget));
	return Owner->GetDistanceTo(CombatTarget) < AttackAvailableDistance;
}

bool UWandererCombatComponent::IsTargetInDashRange() const
{
	if(!CombatTarget) return false;
	if(CombatTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Dead)) return false;
	
	return Owner->GetDistanceTo(CombatTarget) < DashAvailableDistance;
}

bool UWandererCombatComponent::CanDashTo(const AWandererBaseCharacter* DashTarget) const 
{
	return DashTarget ? Owner->GetDistanceTo(DashTarget) < DashAvailableDistance : false; 
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

bool UWandererCombatComponent::CanFinishTarget() const
{
	if(!IsTargetInAttackRange()) return false;

	if(WandererUtils::EvaluateDirectionRelativeToActor(Owner.Get(), CombatTarget.Get()) == EDirection::Backward)
	{
		if(!CombatTarget->GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::State_Combat)) return true;
	}
	
	if((CombatTarget->GetHealthAttributeSet()->GetHealth() / CombatTarget->GetHealthAttributeSet()->GetMaxHealth()) < 0.2f)
	{
		return UKismetMathLibrary::RandomBoolWithWeight(Owner->GetCombatAttributeSet()->GetFinisherChance());
	}

	return false;
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
	OnTargetChanged.Broadcast(CombatTarget);
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
	if(!Owner->IsPlayingRootMotion())
	{
		if(FVector::DotProduct(Forward, ToTarget) < 0.999f)
		{
			Owner->SetActorRotation(FMath::RInterpTo(Forward.Rotation(), ToTarget.Rotation(), DeltaTime, 5.0f));
		}
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
		Weapon->InitializeWithOwner(Owner.Get());
	}
}


