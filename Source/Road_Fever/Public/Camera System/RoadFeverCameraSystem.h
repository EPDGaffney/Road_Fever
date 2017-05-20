// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Structs/CameraPositioning.h"
#include "RoadFeverCameraSystem.generated.h"

UCLASS()
class ROAD_FEVER_API ARoadFeverCameraSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	/*
	*	Setup the root component so this Actor has world positioning.
	*	Setup a trigger area so that when the player enters/leaves it, the appropriate function gets called.
	*	Setup a camera so that one can see in the editor where that camera points to.
	*	11/4/2017 - Matthew Woolley
	*/
	ARoadFeverCameraSystem();

	/*
	*	Calls any BP implemented version of this function.
	*	Stores the camera's position in a variable so that it can be accessed later.
	*	Destroys the camera from memory.
	*	If the camera is the primary one in the level (the camera used when the level loads), use it straight away.
	*	11/4/2017 - Matthew Woolley
	*/
	virtual void BeginPlay() override;

	/*
	 *	Switch the current camera being used, to this one.
	 *	20/5/2017 - Matthew Woolley
	 */
	UFUNCTION( BlueprintCallable, Category = "" )
	void SwitchTo();

	/*
	 *	Switches the current camera being used, back to the one being used before this one.
	 *	20/5/2017 - Matthew Woolley
	 */
	UFUNCTION( BlueprintCallable, Category = "" )
	void SwitchBack();

	// The camera that gets shown in the editor to help the LDs position them. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere )
	class UCameraComponent* EditorCameraReference;

	// Whether or not the game starts using this camera. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	bool bPrimaryCamera;

	// The area that, when entered, triggers the Character's camera location. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Detection" )
	class UBoxComponent* TriggerArea;

private:
	/*
	*	Sets the position of the character's camera so that it uses the location setup by the camera placed in the editor.
	*	Uses the ReverTo variable for when the player leaves this camera detector.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION()
	void OnActorEnter( class UPrimitiveComponent* InPrimitiveComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherPrimitiveComponent, int32 InInt, bool InSweepResult, const FHitResult& InFHit );

	/*
	*	De-assigns the RevertTo variable if its value is that of this camera.
	*	If the RevertTo variable refers to a different camera, and this is the camera in use, set the camera in the RevertTo variabla to be the one being used.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION()
	void OnActorLeave( class UPrimitiveComponent* InPrimitiveCompnent, AActor* InOtherActor, UPrimitiveComponent* InOtherPrimitiveComponent, int32 InInt );

	// The transform that, when triggered, the Character's camera moves to. [11/12/2015 Matthew Woolley]
	UPROPERTY()
	FCameraPositioning CameraPosition;
};
