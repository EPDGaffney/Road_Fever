// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "CameraPositioning.generated.h"


// Handles the position of the camera in-game. [11/12/2015 Matthew Woolley]
USTRUCT( BlueprintType )
struct FCameraPositioning
{
	GENERATED_USTRUCT_BODY()

public:
	// The location for this camera. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	FVector Location;

	// The rotation of this camera. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	FRotator Rotation;
};