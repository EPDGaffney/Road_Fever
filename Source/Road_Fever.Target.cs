// Copyright Square Crank Games. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Road_FeverTarget : TargetRules
{
	public Road_FeverTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "Road_Fever" } );
	}
}
