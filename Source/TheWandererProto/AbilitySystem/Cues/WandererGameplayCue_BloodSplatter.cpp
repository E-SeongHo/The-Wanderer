//  


#include "WandererGameplayCue_BloodSplatter.h"

bool UWandererGameplayCue_BloodSplatter::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	check(BloodSplatterClass);
	check(MyTarget);

	MyTarget->GetWorld()->SpawnActor<AActor>(BloodSplatterClass, Parameters.EffectContext.GetHitResult()->ImpactPoint, FRotator::ZeroRotator);
	return true;	
}
