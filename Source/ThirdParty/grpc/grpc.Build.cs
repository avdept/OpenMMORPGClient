using UnrealBuildTool;
using System;
using System.IO;

public class Grpc : ModuleRules
{
	public Grpc(ReadOnlyTargetRules Target)
		: base(Target)
	{
        Type = ModuleType.External;
        
        //PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
	        return;
        } 
        
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
	        PublicSystemLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Win64"));
	        //PublicAdditionalLibraries.Add("grpc.lib");
            //PublicAdditionalLibraries.Add("grpc++.lib");
            //PublicAdditionalLibraries.Add("gpr.lib");
            //PublicAdditionalLibraries.Add("cares.lib");
            //PublicAdditionalLibraries.Add("upb.lib");
            //PublicAdditionalLibraries.Add("address_sorting.lib");
            //PublicAdditionalLibraries.Add("zlibstatic.lib");
        }
        
        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
        //PublicDefinitions.Add("GOOGLE_PROTOBUF_USE_UNALIGNED=0");
        //PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE=0");
	}
}

