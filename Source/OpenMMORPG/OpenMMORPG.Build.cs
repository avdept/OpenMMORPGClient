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
		AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
		
		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "openssl", "include"));
		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "grpc", "include"));


		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string ProtobufLibrariesPath = Path.Combine(ThirdPartyPath, "protobuf", "lib", "Win64");
			string OpenSSLLibrariesPath = Path.Combine(ThirdPartyPath, "openssl", "lib");
			string GrpcLibrariesPath = Path.Combine(ThirdPartyPath, "grpc", "lib", "Win64");
			
			
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "address_sorting.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "cares.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "gpr.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "grpc.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "grpc++.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "upb.lib"));
			
			PublicAdditionalLibraries.Add(Path.Combine(ProtobufLibrariesPath, "libprotobuf.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libcrypto.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libssl.lib"));

			RuntimeDependencies.Add(Path.Combine(OpenSSLLibrariesPath, "libcrypto-3.dll"));
		}

		if (Target.Platform == UnrealTargetPlatform.Mac) {
			string ProtobufLibrariesPath = Path.Combine(ThirdPartyPath, "protobuf", "lib", "Mac");
			string OpenSSLLibrariesPath = Path.Combine(ThirdPartyPath, "openssl", "lib", "Mac");
			PublicAdditionalLibraries.Add(Path.Combine(ProtobufLibrariesPath, "libprotobuf.a"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libcrypto.1.1.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libssl.1.1.dylib"));
		}
		
		PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
		PublicDefinitions.Add("GOOGLE_PROTOBUF_USE_UNALIGNED=0");
		PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE=0");
	}
}
