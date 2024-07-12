//  

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "WandererAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


// WandererAttributeSet will be initiated by GamePlayEffect
UCLASS()
class THEWANDERERPROTO_API UWandererAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	// does OnRep Function needed? -> If i want to expand my project to multiplayer. Let's think about it
	
};
