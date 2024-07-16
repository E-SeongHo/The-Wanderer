//  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Components/ActorComponent.h"
#include "WandererActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEWANDERERPROTO_API UWandererActorComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UWandererActorComponent();

	void AssignAbilitySystemComponent(UAbilitySystemComponent* OwnerASC);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent.Get(); }

protected:
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
