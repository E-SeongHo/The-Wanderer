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

UWandererMontagePair* UWandererAnimMontageConfig::FindAnimMontagePairForTag(const FGameplayTag& ActionTag) const
{
	for(const FWandererActionMontagePairGroup& MontagePairGroup : MontagePairGroups)
	{
		// e.g) FindAnimMontageForTag(ActionTag.Attack.LeftLead) can return ActionTag.Attack
		if(!MontagePairGroup.MontagePairs.IsEmpty() && ActionTag.MatchesTag(MontagePairGroup.ActionTag)) // include parent tag
		{
			// TODO: Note - While I believe this, it should be monitored. see WandererGameplayAbility_Attack.cpp 
			// I don't think object pooling is necessary as this function is rarely called.
			// Regarding the lifecycle, even if GC sweeps this object, it won't cause a dangling pointer problem
			// once the Data (AnimMontage) is referenced elsewhere, as the anim montage itself will remain alive.
			UWandererMontagePair* MontagePairWrapper = NewObject<UWandererMontagePair>();
			MontagePairWrapper->Data = MontagePairGroup.MontagePairs[FMath::RandRange(0, MontagePairGroup.MontagePairs.Num()-1)];
			MontagePairWrapper->ActionTag = ActionTag;

			return MontagePairWrapper;
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
