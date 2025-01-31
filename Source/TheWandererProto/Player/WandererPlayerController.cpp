// Fill out your copyright notice in the Description page of Project Settings.


#include "WandererPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Character/WandererCharacter.h"
#include "Input/WandererInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "WandererPlayerState.h"
#include "AbilitySystem/Abilities/IRetriggerable.h"
#include "AbilitySystem/Abilities/WandererGameplayAbility.h"
#include "UI/WandererWidget.h"

void AWandererPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(WandererMappingContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(WandererMappingContext, 0);
	}

	if(PlayerWidgetClass)
	{
		UWandererWidget* PlayerWidget = CreateWidget<UWandererWidget>(this, PlayerWidgetClass);
		PlayerWidget->AssignAbilitySystemComponent(GetAbilitySystemComponent());
		PlayerWidget->AddToViewport();
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
		WandererIC->BindNativeAction(InputConfig, WandererGameplayTags::InputTag_ModifyAttack, ETriggerEvent::Started, this, &ThisClass::Input_ModifyAttack);
		WandererIC->BindNativeAction(InputConfig, WandererGameplayTags::InputTag_ModifyAttack, ETriggerEvent::Completed, this, &ThisClass::Input_ModifyAttack);
		
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

void AWandererPlayerController::Input_ModifyAttack(const FInputActionValue& Value)
{
	// TODO: If AI needs to use the modify attack mechanism, consider having the combat component hold this information and check it instead.
	if(!GetAbilitySystemComponent()->HasMatchingGameplayTag(WandererGameplayTags::InputTag_ModifyAttack))
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(WandererGameplayTags::InputTag_ModifyAttack);
	}
	else
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(WandererGameplayTags::InputTag_ModifyAttack);
	}
}

void AWandererPlayerController::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Input Tag Pressed: %s"), *InputTag.ToString()));	

	if (!InputTag.IsValid()) return;
	check(GetAbilitySystemComponent());

	GetAbilitySystemComponent()->AddLooseGameplayTag(InputTag);
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
				// Up until now, all abilities are set as instanced per actor.
				// AbilitySpec.Ability always returns CDO!  
				check(AbilitySpec.ActiveCount == 1);
				IRetriggerable* RetriggerableAbility = Cast<IRetriggerable>(AbilitySpec.GetPrimaryInstance());
				if(RetriggerableAbility)
				{
					if(RetriggerableAbility->CanRetrigger())
					{
						RetriggerableAbility->SaveCurrentContext();
						UE_LOG(LogTemp, Log, TEXT("RETRIGGER"));	
						GetAbilitySystemComponent()->TryActivateAbility(AbilitySpec.Handle);
					}
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

	GetAbilitySystemComponent()->RemoveLooseGameplayTag(InputTag);
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
			//GetAbilitySystemComponent()->AbilitySpecInputPressed(AbilitySpec);
		}
	}
}

UAbilitySystemComponent* AWandererPlayerController::GetAbilitySystemComponent() const
{
	const AWandererPlayerState* WandererPS = GetPlayerState<AWandererPlayerState>();
	return WandererPS ? WandererPS->GetAbilitySystemComponent() : nullptr;
}