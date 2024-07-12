//  


#include "WandererAnimNotifyState_MontageEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "WandererGameplayTags.h"
#include "Character/WandererBaseCharacter.h"

void UWandererAnimNotifyState_MontageEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());

	IAbilitySystemInterface* GASInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	// Do not make assertion here : it is calling even at previewing montage
	if(GASInterface)
	{
		check(Cast<AWandererBaseCharacter>(GASInterface->GetAbilitySystemComponent()->GetAvatarActor()));
		GASInterface->GetAbilitySystemComponent()->AddLooseGameplayTag(StateTag);
	}
}

void UWandererAnimNotifyState_MontageEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	IAbilitySystemInterface* GASInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	// Do not make assertion here : it is calling even at previewing montage
	if(GASInterface)
	{
		check(Cast<AWandererBaseCharacter>(GASInterface->GetAbilitySystemComponent()->GetAvatarActor()));
		GASInterface->GetAbilitySystemComponent()->RemoveLooseGameplayTag(StateTag);
	}
}


