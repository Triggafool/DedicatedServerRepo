﻿using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class FPSTemplateClientTarget : TargetRules
{
	public FPSTemplateClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("FPSTemplate");
		ExtraModuleNames.Add("DedicatedServers");
	}
}