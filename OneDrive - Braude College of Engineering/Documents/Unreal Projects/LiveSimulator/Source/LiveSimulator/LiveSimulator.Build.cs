// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LiveSimulator : ModuleRules
{
	public LiveSimulator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LiveSimulator",
			"LiveSimulator/Variant_Platforming",
			"LiveSimulator/Variant_Platforming/Animation",
			"LiveSimulator/Variant_Combat",
			"LiveSimulator/Variant_Combat/AI",
			"LiveSimulator/Variant_Combat/Animation",
			"LiveSimulator/Variant_Combat/Gameplay",
			"LiveSimulator/Variant_Combat/Interfaces",
			"LiveSimulator/Variant_Combat/UI",
			"LiveSimulator/Variant_SideScrolling",
			"LiveSimulator/Variant_SideScrolling/AI",
			"LiveSimulator/Variant_SideScrolling/Gameplay",
			"LiveSimulator/Variant_SideScrolling/Interfaces",
			"LiveSimulator/Variant_SideScrolling/UI",
			"LiveSimulator/Variant_Childhood"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
