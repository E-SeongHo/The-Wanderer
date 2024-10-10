// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TheWandererProtoTarget : TargetRules
{
	public TheWandererProtoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		bOverrideBuildEnvironment = true;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("TheWandererProto");
	}
}
