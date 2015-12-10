// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "RoadFeverCharacterNed.generated.h"

UCLASS()
class ROAD_FEVER_API ARoadFeverCharacterNed : public ACharacter
{
	GENERATED_BODY()

public:
	// Called when this Character enters memory. [10/12/2015 Matthew Woolley]
	ARoadFeverCharacterNed();

	// Called to bind player input. [10/12/2015 Matthew Woolley]
	virtual void SetupPlayerInputComponent( class UInputComponent* InInputComponent ) override;
};
