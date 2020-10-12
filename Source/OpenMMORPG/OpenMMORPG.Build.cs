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

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets", "Http" });

		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Slate", "SlateCore", "Json", "JsonUtilities"});

		string ProtobufPath = Path.Combine(ThirdPartyPath, "protobuf", "include");

		PublicIncludePaths.Add(ProtobufPath);
		AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");

		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "openssl", "include"));
		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "grpc", "include"));


		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string ProtobufLibrariesPath = Path.Combine(ThirdPartyPath, "protobuf", "lib", "Win64");
			string OpenSSLLibrariesPath = Path.Combine(ThirdPartyPath, "openssl", "lib", "Win64");
			string GrpcLibrariesPath = Path.Combine(ThirdPartyPath, "grpc", "lib", "Win64");
			string AbseilPath = Path.Combine(ThirdPartyPath, "abseil", "lib", "Win64");
			string RE2Path = Path.Combine(ThirdPartyPath, "re2", "lib", "Win64");
			
			//Abseil-cpp
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_strings.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_strings_internal.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_str_format_internal.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_synchronization.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_status.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_base.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_spinlock_wait.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_throw_delegate.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_time.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_bad_optional_access.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_cord.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_raw_logging_internal.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_graphcycles_internal.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_malloc_internal.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_symbolize.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_time_zone.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_int128.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(AbseilPath, "absl_stacktrace.lib"));
			//Re2
			PublicAdditionalLibraries.Add(Path.Combine(RE2Path, "re2.lib"));

			//GRPC
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "address_sorting.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "cares.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "gpr.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "grpc.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "grpc++.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "upb.lib"));

			//Protobuf
			PublicAdditionalLibraries.Add(Path.Combine(ProtobufLibrariesPath, "libprotobuf.lib"));
			//BoringSSL
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "crypto.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "ssl.lib"));


		}

		if (Target.Platform == UnrealTargetPlatform.Mac) {

			string GrpcLibrariesPath = Path.Combine(ThirdPartyPath, "grpc", "lib", "Mac");
			string ProtobufLibrariesPath = Path.Combine(ThirdPartyPath, "protobuf", "lib", "Mac");
			string OpenSSLLibrariesPath = Path.Combine(ThirdPartyPath, "openssl", "lib", "Mac");
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "libaddress_sorting.a"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "libcares.a"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "libgpr.a"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "libgrpc.a"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "libgrpc++.a"));
			PublicAdditionalLibraries.Add(Path.Combine(GrpcLibrariesPath, "libupb.a"));


			PublicAdditionalLibraries.Add(Path.Combine(ProtobufLibrariesPath, "libprotobuf.a"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libcrypto.1.1.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(OpenSSLLibrariesPath, "libssl.1.1.dylib"));
		}

		PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
		PublicDefinitions.Add("GOOGLE_PROTOBUF_USE_UNALIGNED=0");
		PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE=0");
	}
}
