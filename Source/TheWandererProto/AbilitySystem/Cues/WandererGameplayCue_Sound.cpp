//  


#include "AbilitySystem/Cues/WandererGameplayCue_Sound.h"

#include "Character/WandererBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

bool UWandererGameplayCue_Sound::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	USoundBase* SoundToPlay = SoundBase ? SoundBase.Get() : Cast<AWandererBaseCharacter>(MyTarget)->RequestSoundBaseForGameplayCue(GameplayCueTag);

	// Make sure GameplayCue having HitTarget, HitImpactPoint Type, invoked from appropriate GameplayEffect
	FVector SoundLocation;
	switch(SoundPlayLocationType)
	{
	case EWandererSoundPlayLocation::Instigator:
		SoundLocation = MyTarget->GetActorLocation();
		break;
	case EWandererSoundPlayLocation::HitTarget:
		check(Parameters.EffectContext.GetHitResult());
		SoundLocation = Parameters.EffectContext.GetHitResult()->GetActor()->GetActorLocation();
		break;
	case EWandererSoundPlayLocation::HitImpactPoint:
		check(Parameters.EffectContext.GetHitResult());
		SoundLocation = Parameters.EffectContext.GetHitResult()->ImpactPoint;
		break;
	default:
		SoundLocation = MyTarget->GetActorLocation();
		break;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, SoundLocation);
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
