//  

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "WandererGameplayCue_BloodSplatter.generated.h"

/**
 * This class should not have a GameplayCueTag assigned directly in Cpp file as it's not going to work without BloodSplatter actor. 
 * Instead, assign the tag as needed for proper use after creating a BP class inheriting from this and binding the BloodSplatterClass in the editor.
 * It has designed in order to reuse pre-made Niagara spawner actor.
 */
UCLASS()
class THEWANDERERPROTO_API UWandererGameplayCue_BloodSplatter : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<AActor> BloodSplatterClass;
};
