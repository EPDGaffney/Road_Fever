// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverGameMode.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



// Called when this GameMode enters memory. [11/12/2015 Matthew Woolley]
ARoadFeverGameMode::ARoadFeverGameMode()
{
	DefaultPawnClass = ARoadFeverCharacterNed::StaticClass();
}
