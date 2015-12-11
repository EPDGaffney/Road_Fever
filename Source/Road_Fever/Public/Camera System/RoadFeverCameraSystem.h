// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RoadFeverCameraSystem.generated.h"

UCLASS()
class ROAD_FEVER_API ARoadFeverCameraSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called when this Actor enters memory. [11/12/2015 Matthew Woolley]
	ARoadFeverCameraSystem();

	// Called at the beginning of game-play. [11/12/2015 Matthew Woolley]
	virtual void BeginPlay() override;	
	
};
