// Fill out your copyright notice in the Description page of Project Settings.


#include "WandererCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MotionWarpingComponent.h"
#include "WandererGameplayTags.h"
#include "../../../../../../Installed Files/UnrealEngine/UE_5.4/Engine/Plugins/Experimental/Animation/MotionTrajectory/Source/MotionTrajectory/Public/CharacterTrajectoryComponent.h"
#include "AbilitySystem/Abilities/WandererActiveGameplayAbility.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "Player/WandererPlayerState.h"
#include "Weapon/WandererSword.h"

AWandererCharacter::AWandererCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f; 
	SpringArmComponent->bUsePawnControlRotation = true; 

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); 
	CameraComponent->bUsePawnControlRotation = false; 

	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("Trajectory"));
}

void AWandererCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ASC 
	AWandererPlayerState* WandererPS = GetPlayerState<AWandererPlayerState>();
	check(WandererPS);
	
	WandererPS->GetAbilitySystemComponent()->InitAbilityActorInfo(WandererPS, this);
	AbilitySystemComponent = WandererPS->GetAbilitySystemComponent();
	HealthAttributeSet = WandererPS->GetHealthAttributeSet();
	CombatAttributeSet = WandererPS->GetCombatAttributeSet();
	
	GrantStartupAbilities();
	
	// Default Gameplay Tag
	AbilitySystemComponent->AddLooseGameplayTag(WandererGameplayTags::State_Weapon_Unarmed);

	// Init Default Attributes By GE
	const UGameplayEffect* InitterGE = DefaultAttributesInitter->GetDefaultObject<UGameplayEffect>();
	AbilitySystemComponent->ApplyGameplayEffectToSelf(InitterGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void AWandererCharacter::AttachWeaponMeshToSocket(FName SocketName)
{
	check(Weapon);
	Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

void AWandererCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(SwordType)
	{
		Weapon = GetWorld()->SpawnActor<AWandererSword>(SwordType, FVector::ZeroVector, FRotator::ZeroRotator);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("SheathSocket"));
		AttachWeaponMeshToSocket(TEXT("SheathSocket"));
	}
}

/*void AWandererCharacter::Vault(const FInputActionValue& Value)
{
	FVector VaultStart, VaultMiddle, VaultLand;

	FHitResult HitResult;
	auto SphereTrace = [this, &HitResult](const FVector& Start, const FVector& End) -> bool
	{
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(this);

		constexpr float TraceRadius = 5.0f;
		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(TraceRadius),
			Param
			);
	
		FColor TraceColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Start, TraceRadius, 12, TraceColor, false, 1.0f);
		DrawDebugSphere(GetWorld(), End, TraceRadius, 12, TraceColor, false, 1.0f);
		DrawDebugLine(GetWorld(), Start, End, TraceColor, false, 1.0f);

		return bHit;
	};

	constexpr float HeightStride = 30.0f;
	constexpr float WidthStride = 50.0f;

	constexpr float ForwardTraceDepth = 180.0f;
	constexpr float DownwardTraceDepth = 100.0f;
	constexpr float GroundTraceDepth = 10000.0f;

	bool bCanWarp = false;

	for(int i = 0; i < 3; i++)
	{
		const FVector ForwardStart = GetActorLocation() + FVector(0.0f, 0.0f, HeightStride * i);
		const FVector ForwardEnd = ForwardStart + GetActorRotation().Vector() * ForwardTraceDepth;

		bool bForwardHit = SphereTrace(ForwardStart, ForwardEnd);

		if(bForwardHit)
		{
			const FVector Offset = HitResult.Location + GetActorRotation().Vector() * 5.0f; // add little bit offset 
			for(int j = 0; j < 6; j++)
			{
				const FVector DownwardEnd =  Offset + GetActorRotation().Vector() * WidthStride * j;
				const FVector DownwardStart = DownwardEnd + FVector(0.0f, 0.0f, DownwardTraceDepth);
				
				bool bDownwardHit = SphereTrace(DownwardStart, DownwardEnd);
				
				if(bDownwardHit)
				{
					if(j == 0) VaultStart = HitResult.Location;
					else VaultMiddle = HitResult.Location;
				}
				else// Find Position To Land
				{
					const FVector ToGroundStart = DownwardEnd; // Add little more offset to look nice
					const FVector ToGroundEnd = ToGroundStart - FVector(0.0f, 0.0f, GroundTraceDepth); // until the ground

					bool bGroundHit = SphereTrace(ToGroundStart, ToGroundEnd);
					check(bGroundHit);

					bCanWarp = bGroundHit;
					VaultLand = HitResult.Location;

					if(abs(GetMesh()->GetComponentLocation().Z - VaultLand.Z) > 50.0f)
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

	DrawDebugSphere(GetWorld(), VaultStart, 5.0f, 12, FColor::Yellow, false, 1.0f);
	DrawDebugSphere(GetWorld(), VaultMiddle, 5.0f, 12, FColor::Yellow, false, 1.0f);
	DrawDebugSphere(GetWorld(), VaultLand, 5.0f, 12, FColor::Yellow, false, 1.0f);

	if(bCanWarp)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		PlayAnimMontage(VaultOverAnim);

		MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultStart"), VaultStart, GetActorRotation());
		MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultMiddle"), VaultMiddle, GetActorRotation());
		MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultLand"), VaultLand, GetActorRotation());
		
		FOnMontageEnded BlendOutDelegate;
		BlendOutDelegate.BindLambda([this, &bCanWarp, &VaultLand](UAnimMontage* Montage, bool bInterrupted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage ended. Interrupted: %s"), bInterrupted ? TEXT("true") : TEXT("false"));
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			bCanWarp = false;
			VaultLand = FVector(0.0f, 0.0f, 20000.0f);
		});

		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(BlendOutDelegate);
	}
}*/

void AWandererCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

