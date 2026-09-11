// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Xersuo : ModuleRules
{
	public Xersuo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities","GameplayTags", "GameplayTasks" });

		PublicIncludePaths.AddRange(new string[] {
			"Xersuo",
		});
	}
}
