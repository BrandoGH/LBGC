// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class LBGC : ModuleRules
{
	public LBGC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Sockets", "Networking", "XmlParser", "AIModule" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // macro
        PublicDefinitions.Add("LBGS_CONFIG_DIR=FString::Printf(TEXT(\"%sLBGC/ConfigFile/\"),*FString(FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir())))");
        PublicDefinitions.Add("LBGS_DEBUG=1");

        // Package the game and add the configuration file to the game directory
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "ConfigFile", "server.xml"));

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
