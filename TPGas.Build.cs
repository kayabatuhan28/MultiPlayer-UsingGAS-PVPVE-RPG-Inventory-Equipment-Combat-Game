// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPGas : ModuleRules
{
	public TPGas(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara", 
			"EnhancedInput",
			"UMG", 
			"NetCore",
			"StructUtils",
			"Slate",
			"SlateCore"
		});
	}
}
