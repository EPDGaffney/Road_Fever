// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverGameMode.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



// Called when this GameMode enters memory. [11/12/2015 Matthew Woolley]
ARoadFeverGameMode::ARoadFeverGameMode()
{
	// Get the Pawn class from the content browser. [16/12/2015 Matthew Woolley]
	static ConstructorHelpers::FClassFinder<APawn> PawnClassObj( TEXT( "Blueprint'/Game/RoadFever/Ned/Ned.Ned_C'" ) );

	// Implement it into the game. [16/12/2015 Matthew Woolley]
	DefaultPawnClass = PawnClassObj.Class;
}

bool ARoadFeverGameMode::SetPause( APlayerController* PC, FCanUnpause CanUnpauseDelegate /* = FCanUnpause() */ )
{
	return PC->IsPaused();
}
