//  


#include "WandererAnimNotify_MontageEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Character/WandererBaseCharacter.h"

class IAbilitySystemInterface;

void UWandererAnimNotify_MontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(EventTag.IsValid());
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());

	if(StateTag.IsValid())
	{
		IAbilitySystemInterface* GASInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
		if(GASInterface)
		{
			check(Cast<AWandererBaseCharacter>(GASInterface->GetAbilitySystemComponent()->GetAvatarActor()));
			GASInterface->GetAbilitySystemComponent()->AddLooseGameplayTag(StateTag);
		}
	}
}
