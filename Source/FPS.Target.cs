// Copyright by Vishnu Suresh

using UnrealBuildTool;
using System.Collections.Generic;

public class FPSTarget : TargetRules
{
	public FPSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "FPS" } );
	}
}
