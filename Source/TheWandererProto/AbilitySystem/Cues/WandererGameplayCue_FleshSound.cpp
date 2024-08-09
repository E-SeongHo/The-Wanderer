//  


#include "WandererGameplayCue_FleshSound.h"

#include "Kismet/GameplayStatics.h"

bool UWandererGameplayCue_FleshSound::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FleshSounds[FMath::RandRange(0, FleshSounds.Num()-1)], Parameters.EffectContext.GetHitResult()->ImpactPoint);

	return true;
}
