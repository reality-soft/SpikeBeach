// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpikeBeachEditorTarget : TargetRules
{
	public SpikeBeachEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        bLegacyPublicIncludePaths = false;
        ExtraModuleNames.Add("SpikeBeach");
	}
}
