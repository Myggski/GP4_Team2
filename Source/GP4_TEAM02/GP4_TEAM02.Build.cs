// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GP4_TEAM02 : ModuleRules
{
	public GP4_TEAM02(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore" });
	}
}
