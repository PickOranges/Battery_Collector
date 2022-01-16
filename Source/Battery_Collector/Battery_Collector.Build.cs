// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Battery_Collector : ModuleRules
{
	public Battery_Collector(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		// Added UMG in order to use user widget class in your code. 2021.12.06
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
		
	}
}
