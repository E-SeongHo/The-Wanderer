// Fill out your copyright notice in the Description page of Project Settings.


#include "WandererPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Character/WandererCharacter.h"
#include "Input/WandererInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "WandererPlayerState.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"

void AWandererPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(WandererMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(WandererMappingContext, 0);
	}
}

void AWandererPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UWandererInputComponent* WandererIC = CastChecked<UWandererInputComponent>(InputComponent))
	{
		check(InputConfig);
		
		// Bind Native Actions
		WandererIC->BindNativeAction(InputConfig, WandererGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		WandererIC->BindNativeAction(InputConfig, WandererGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
		WandererIC->BindNativeAction(InputConfig, WandererGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);
		WandererIC->BindNativeAction(InputConfig, WandererGameplayTags::InputTag_Crouch, ETriggerEvent::Completed, this, &ThisClass::Input_UnCrouch);
		
		// Bind Ability Actions
		WandererIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, &ThisClass::Input_AbilityInputTagHeld);
	}
}

void AWandererPlayerController::Input_Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWandererPlayerController::Input_Look(const FInputActionValue& Value) 
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);	
	}
}

void AWandererPlayerController::Input_Crouch(const FInputActionValue& Value)
{
	if(ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Crouch();
	}
}

void AWandererPlayerController::Input_UnCrouch(const FInputActionValue& Value)
{
	if(ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->UnCrouch();
	}
}

void AWandererPlayerController::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Input Tag Pressed: %s"), *InputTag.ToString()));	

	if (!InputTag.IsValid()) return;
	check(GetAbilitySystemComponent());
	
	for (FGameplayAbilitySpec& AbilitySpec : GetAbilitySystemComponent()->GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				GetAbilitySystemComponent()->AbilitySpecInputPressed(AbilitySpec);
				GetAbilitySystemComponent()->TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				if(Cast<UWandererGameplayAbility>(AbilitySpec.Ability)->CanRetrigger())
				{
					GetAbilitySystemComponent()->TryActivateAbility(AbilitySpec.Handle);
				}
				else
				{
					GetAbilitySystemComponent()->AbilitySpecInputPressed(AbilitySpec);
				}
			}
		}
	}
}

void AWandererPlayerController::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	check(GetAbilitySystemComponent());

	for (FGameplayAbilitySpec& AbilitySpec : GetAbilitySystemComponent()->GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			GetAbilitySystemComponent()->AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void AWandererPlayerController::Input_AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	check(GetAbilitySystemComponent());

	for (FGameplayAbilitySpec& AbilitySpec : GetAbilitySystemComponent()->GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//AbilitySystemComponent->AbilitySpecInputPressed(AbilitySpec);
		}
	}
}

UAbilitySystemComponent* AWandererPlayerController::GetAbilitySystemComponent() const
{
	const AWandererPlayerState* WandererPS = GetPlayerState<AWandererPlayerState>();
	return WandererPS ? WandererPS->GetAbilitySystemComponent() : nullptr;
}