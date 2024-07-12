//  


#include "WandererAnimNotify_MontageEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

class IAbilitySystemInterface;

void UWandererAnimNotify_MontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());
}
