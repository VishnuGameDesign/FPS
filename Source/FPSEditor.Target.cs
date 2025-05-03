// Copyright by Vishnu Suresh

using UnrealBuildTool;
using System.Collections.Generic;

public class FPSEditorTarget : TargetRules
{
	public FPSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "FPS" } );
	}
}
