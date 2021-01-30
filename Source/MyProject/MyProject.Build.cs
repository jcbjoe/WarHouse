// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
	public MyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "CableComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });
		PrivateIncludePathModuleNames.AddRange(new string[] { "CableComponent" });
	}
}