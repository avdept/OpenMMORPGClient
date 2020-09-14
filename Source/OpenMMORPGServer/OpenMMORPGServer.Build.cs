using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

public class OpenMMORPGServer : ModuleRules
{
	public List<string> GetSubDirectories(string root)
	{
		// Get all subdirectories
		List<string> allSubdirectoryEntries = new List<string>();
		allSubdirectoryEntries.Add(root);

		// Loop through them to see if they have any other subdirectories
		string[] rootDirEntries = Directory.GetDirectories(root);
		foreach (string subdirectory in rootDirEntries)
			LoadSubDirs(subdirectory, ref allSubdirectoryEntries);

		return allSubdirectoryEntries;
	}
	private void LoadSubDirs(string dir, ref List<string> allSubdirectoryEntries)
	{
		allSubdirectoryEntries.Add(dir);
		string[] subdirectoryEntries = System.IO.Directory.GetDirectories(dir);
		foreach (string subdirectory in subdirectoryEntries)
		{
			LoadSubDirs(subdirectory, ref allSubdirectoryEntries);
		}
	}

	public OpenMMORPGServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"OpenMMORPG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {

		});

		List<string> allPublicDirectories = new List<String>();
		List<string> allPrivateDirectories = new List<String>();;

		if (Target.Platform == UnrealTargetPlatform.Mac) {
			allPublicDirectories = GetSubDirectories(ModuleDirectory + @"/Public/");
			allPrivateDirectories = GetSubDirectories(ModuleDirectory + @"/Private/");
		}

		if (Target.Platform == UnrealTargetPlatform.Win64) {
			allPublicDirectories = GetSubDirectories(ModuleDirectory + @"\Public\");
			allPrivateDirectories = GetSubDirectories(ModuleDirectory + @"\Private\");
		}

		PublicIncludePaths.AddRange(allPublicDirectories);
		PrivateIncludePaths.AddRange(allPrivateDirectories);
		PrivateDependencyModuleNames.Add("OnlineSubsystem");
	}
}
