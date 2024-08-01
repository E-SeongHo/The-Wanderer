//  


#include "WandererAnimMontageConfig.h"

UAnimMontage* UWandererAnimMontageConfig::FindAnimMontageForTag(const FGameplayTag& ActionTag) const
{
	for(const FWandererActionMontageGroup& MontageGroup : MontageGroups)
	{
		// e.g) FindAnimMontageForTag(ActionTag.Attack.LeftLead) can return ActionTag.Attack
		if(!MontageGroup.Montages.IsEmpty() && ActionTag.MatchesTag(MontageGroup.ActionTag)) // include parent tag
		{
			return MontageGroup.Montages[FMath::RandRange(0, MontageGroup.Montages.Num()-1)];
		}
	}
	
	return nullptr;
}

bool UWandererAnimMontageConfig::HasExactMatchingActionMontage(const FGameplayTag& ActionTag) const
{
	for(const FWandererActionMontageGroup& MontageGroup : MontageGroups)
	{
		if(!MontageGroup.Montages.IsEmpty() && MontageGroup.ActionTag.MatchesTagExact(ActionTag))
		{
			return true;
		}
	}

	return false;
}
