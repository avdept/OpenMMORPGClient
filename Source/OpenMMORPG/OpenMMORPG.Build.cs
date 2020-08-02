// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class OpenMMORPG : ModuleRules
{


	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty")); }
	}
	public OpenMMORPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets" });

		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore"});
		
		string ProtobufPath = Path.Combine(ThirdPartyPath, "protobuf", "include");
		
		PublicIncludePaths.Add(ProtobufPath);
		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "openssl", "include"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string ProtobufLibrariesPath = Path.Combine(ThirdPartyPath, "protobuf", "lib", "Win64");
			string OpenSSLLibrariesPath = Path.Combine(ThirdPartyPath, "openssl", "lib");
		
			PublicAdditionalLibraries.Add(Path.Combine(ProtobufLibrariesPath, "libprotobuf.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libcrypto.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libssl.lib"));

			RuntimeDependencies.Add(Path.Combine(OpenSSLLibrariesPath, "libcrypto-3.dll"));
		}
	}
}
