// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkillSystem : ModuleRules
{
	public SkillSystem(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"GameplayTags",
			"UMG",                    
			"ModelViewViewModel"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara", "Niagara" });

		PublicIncludePaths.AddRange(new string[] {
			"SkillSystem",
			"SkillSystem/Variant_Platforming",
			"SkillSystem/Variant_Platforming/Animation",
			"SkillSystem/Variant_Combat",
			"SkillSystem/Variant_Combat/AI",
			"SkillSystem/Variant_Combat/Animation",
			"SkillSystem/Variant_Combat/Gameplay",
			"SkillSystem/Variant_Combat/Interfaces",
			"SkillSystem/Variant_Combat/UI",
			"SkillSystem/Variant_SideScrolling",
			"SkillSystem/Variant_SideScrolling/AI",
			"SkillSystem/Variant_SideScrolling/Gameplay",
			"SkillSystem/Variant_SideScrolling/Interfaces",
			"SkillSystem/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
