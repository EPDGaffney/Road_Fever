// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "CameraPositioning.generated.h"


/*
 *	Stores the location and rotation of any-given camera placed in the level.
 *	11/4/2017 - Matthew Woolley
 */
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