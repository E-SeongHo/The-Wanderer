// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheWandererProto : ModuleRules
{
	public TheWandererProto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "UMG", 
			"HeadMountedDisplay", "EnhancedInput", "MotionWarping",
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
			"MotionTrajectory", "Niagara", "AIModule", "PoseSearch"
		});
        
        PublicIncludePaths.AddRange(new string[] {
        		"TheWandererProto"
        });
	}
	
	
}
