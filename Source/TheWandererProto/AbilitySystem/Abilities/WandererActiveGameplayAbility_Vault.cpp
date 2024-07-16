//  


#include "WandererActiveGameplayAbility_Vault.h"

#include "MotionWarpingComponent.h"
#include "WandererGameplayTags.h"
#include "Character/WandererCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Utility/WandererUtils.h"

UWandererActiveGameplayAbility_Vault::UWandererActiveGameplayAbility_Vault()
	: Super(WandererGameplayTags::InputTag_Vault)
{
}

void UWandererActiveGameplayAbility_Vault::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AWandererCharacter* WandererCharacter = Cast<AWandererCharacter>(ActorInfo->AvatarActor.Get());
	check(WandererCharacter);
	
	constexpr float HeightStride = 30.0f;
	constexpr float WidthStride = 50.0f;

	constexpr float ForwardTraceDepth = 180.0f;
	constexpr float DownwardTraceDepth = 100.0f;
	constexpr float GroundTraceDepth = 10000.0f;

	bCanWarp = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vault!")));	
	for(int i = 0; i < 3; i++)
	{
		const FVector ForwardStart = WandererCharacter->GetActorLocation() + FVector(0.0f, 0.0f, HeightStride * i);
		const FVector ForwardEnd = ForwardStart + WandererCharacter->GetActorRotation().Vector() * ForwardTraceDepth;

		FHitResult HitResult;
		const bool bForwardHit = WandererUtils::SphereTrace(HitResult, WandererCharacter, ForwardStart, ForwardEnd, 5.0f);
		if(bForwardHit)
		{
			const FVector Offset = HitResult.Location + WandererCharacter->GetActorRotation().Vector() * 5.0f; // add little bit offset 
			for(int j = 0; j < 6; j++)
			{
				const FVector DownwardEnd =  Offset + WandererCharacter->GetActorRotation().Vector() * WidthStride * j;
				const FVector DownwardStart = DownwardEnd + FVector(0.0f, 0.0f, DownwardTraceDepth);
				
				const bool bDownwardHit = WandererUtils::SphereTrace(HitResult, WandererCharacter, DownwardStart, DownwardEnd, 5.0f);
				if(bDownwardHit)
				{
					if(j == 0) VaultStart = HitResult.Location;
					else VaultMiddle = HitResult.Location;
				}
				else// Find Position To Land
				{
					const FVector ToGroundStart = DownwardEnd; // Add little more offset to look nice
					const FVector ToGroundEnd = ToGroundStart - FVector(0.0f, 0.0f, GroundTraceDepth); // until the ground

					bool bGroundHit = WandererUtils::SphereTrace(HitResult, WandererCharacter, ToGroundStart, ToGroundEnd, 5.0f);
					check(bGroundHit);

					bCanWarp = bGroundHit;
					VaultLand = HitResult.Location;

					if(abs(WandererCharacter->GetMesh()->GetComponentLocation().Z - VaultLand.Z) > 50.0f)
					{
						// should make another action ( ex) climbing... )
						UE_LOG(LogTemp, Warning, TEXT("can warp, but landing position is too high or too low"));
						bCanWarp = false;
						return;
					}
					break;
				}
			}
			break;
		}
	}

	if(bCanWarp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Can Vault!")));	

		WandererCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		WandererCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Vault Over"), VaultOverAnim);
		PlayMontageTask->OnCompleted.AddDynamic(this, &UWandererActiveGameplayAbility_Vault::OnMontageCompleted);
		PlayMontageTask->ReadyForActivation();
		
		WandererCharacter->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultStart"), VaultStart, WandererCharacter->GetActorRotation());
		WandererCharacter->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultMiddle"), VaultMiddle, WandererCharacter->GetActorRotation());
		WandererCharacter->GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultLand"), VaultLand, WandererCharacter->GetActorRotation());
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UWandererActiveGameplayAbility_Vault::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AWandererCharacter* WandererCharacter =  Cast<AWandererCharacter>(ActorInfo->AvatarActor.Get());
	check(WandererCharacter);
	
	WandererCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	WandererCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	VaultLand = FVector(0.0f, 0.0f, 20000.0f);
	bCanWarp = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UWandererActiveGameplayAbility_Vault::OnMontageCompleted()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Montage ended")));
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
