// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCameraDummy.h"


// Called when this Actor enters memory. [11/12/2015 Matthew Woolley]
ARoadFeverCameraDummy::ARoadFeverCameraDummy()
{
	// Create a collision for the camera system to detect. [11/12/2015 Matthew Woolley]
	RootComponent = CreateDefaultSubobject<UBoxComponent>( TEXT( "CameraDetector" ) );
	SetActorScale3D( FVector( .25, .25, .25 ) );
}