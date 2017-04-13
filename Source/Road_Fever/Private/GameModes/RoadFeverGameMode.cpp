// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverGameMode.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



/*
 *	Sets the default pawn used by the level.
 *	11/4/2017 - Matthew Woolley
 */
ARoadFeverGameMode::ARoadFeverGameMode()
{
	// Get the Pawn class from the content browser. [16/12/2015 Matthew Woolley]
	static ConstructorHelpers::FClassFinder<APawn> PawnClassObj( TEXT( "Blueprint'/Game/RoadFever/Characters/Ned/BP_Ned.BP_Ned_C'" ) );

	if ( DefaultPawnClass != NULL )
	{
		// Implement it into the game. [16/12/2015 Matthew Woolley]
		DefaultPawnClass = PawnClassObj.Class;
	}
}