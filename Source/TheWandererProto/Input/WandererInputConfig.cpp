// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/WandererInputConfig.h"

const UInputAction* UWandererInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FWandererInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	
	return nullptr;
}

const UInputAction* UWandererInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FWandererInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;	
		}
	}
	
	return nullptr;
}

