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

protected:
	// Moves the Character in the X axis. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void MoveForward( float InInputVal );

	// Moves the Character in the Y axis. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void MoveRight( float InInputVal );

private:
	// Called to preform generic movement function. [10/12/2015 Matthew Woolley]
	UFUNCTION( Category = "Movement" )
	void _move( float InInputVal, EAxis::Type InMoveAxis );
};
