//  


#include "AbilitySystem/Cues/WandererGameplayCueSoundConfig.h"

USoundBase* UWandererGameplayCueSoundConfig::FindSoundBaseForGameplayCueTag(const FGameplayTag& GameplayCueTag) const
{
	for(const FWandererGameplayCueSound& GameplayCueSound : GameplayCueSounds)
	{
		if(GameplayCueSound.SoundBase && GameplayCueTag == GameplayCueSound.GameplayCueTag)
		{
			return GameplayCueSound.SoundBase;
		}
	}

	return nullptr;
}
