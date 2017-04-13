// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCameraSystem.h"
#include "Public/Characters/RoadFeverCharacterNed.h"

// The CameraSystem to go to when leaving this one. [17/6/2016 Matthew Woolley]
ARoadFeverCameraSystem* RevertTo;

/*
 *	Setup the root component so this Actor has world positioning.
 *	Setup a trigger area so that when the player enters/leaves it, the appropriate function gets called.
 *	Setup a camera so that one can see in the editor where that camera points to.
 *	11/4/2017 - Matthew Woolley
 */
ARoadFeverCameraSystem::ARoadFeverCameraSystem()
{
	// Create the class' root component. [11/12/2015 Matthew Woolley]
	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );

	// Create the trigger for this camera. [11/12/2015 Matthew Woolley]
	TriggerArea = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerArea" ) );
	TriggerArea->SetupAttachment( RootComponent );
	TriggerArea->bAbsoluteRotation = true;
	TriggerArea->SetCollisionProfileName( TEXT( "PlayerDetection" ) );
	TriggerArea->OnComponentBeginOverlap.AddDynamic( this, &ARoadFeverCameraSystem::OnActorEnter );
	TriggerArea->OnComponentEndOverlap.AddDynamic( this, &ARoadFeverCameraSystem::OnActorLeave );

	// Create the editor's camera. [11/12/2015 Matthew Woolley]
	EditorCameraReference = CreateDefaultSubobject<UCameraComponent>( TEXT( "EditorCameraReference" ) );
	EditorCameraReference->SetupAttachment( RootComponent );
}


/*
 *	Calls any BP implemented version of this function.
 *	Stores the camera's position in a variable so that it can be accessed later.
 *	Destroys the camera from memory.
 *	If the camera is the primary one in the level (the camera used when the level loads), use it straight away.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCameraSystem::BeginPlay()
{
	Super::BeginPlay();

	// Get the camera's position. [11/12/2015 Matthew Woolley]
	CameraPosition.Location = EditorCameraReference->GetComponentLocation();
	CameraPosition.Rotation = EditorCameraReference->GetComponentRotation();

	// Destroy the camera so that the game doesn't keep rendering unneeded scenes. [11/12/2015 Matthew Woolley]
	EditorCameraReference->DestroyComponent();

	if ( bPrimaryCamera && GetWorld() )
	{
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

		// If the character exists. [11/4/2017 Matthew Woolley]
		if ( PlayerCharacter )
		{
			// Set the camera's position. [11/12/2015 Matthew Woolley]
			PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
			PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
			PlayerCharacter->CurrentCamera = this;
			RevertTo = nullptr;

			// Set the camera's position. [11/12/2015 Matthew Woolley]
			PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
			PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
		}

	}
}

/*
 *	Sets the position of the character's camera so that it uses the location setup by the camera placed in the editor.
 *	Uses the ReverTo variable for when the player leaves this camera detector.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCameraSystem::OnActorEnter( class UPrimitiveComponent* InPrimitiveComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherPrimitiveComponent, int32 InInt, bool InSweepResult, const FHitResult& InFHit )
{
	// If it is the player's camera dummy. [11/12/2015 Matthew Woolley]
	if ( InOtherActor->IsA( ARoadFeverCharacterNed::StaticClass() ) )
	{
		// Get the player's Character. [11/12/2015 Matthew Woolley]
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( InOtherActor );

		if ( PlayerCharacter )
		{
			// If there isn't a Camera in use. [20/1/2016 Matthew Woolley]
			if ( PlayerCharacter->CurrentCamera == nullptr )
			{
				// Set the camera's position. [11/12/2015 Matthew Woolley]
				PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
				PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
				PlayerCharacter->CurrentCamera = this;
				RevertTo = nullptr;
			}
			else
			{
				// Stores the camera that was in use, so that we can revert to it later when leaving this camera's influence. [13/4/2017 Matthew Woolley]
				RevertTo = PlayerCharacter->CurrentCamera;

				// Set the camera's position. [11/12/2015 Matthew Woolley]
				PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
				PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
				PlayerCharacter->CurrentCamera = this;
			}
		}
	}
}

/*
 *	De-assigns the RevertTo variable if its value is that of this camera.
 *	If the RevertTo variable refers to a different camera, and this is the camera in use, set the camera in the RevertTo variabla to be the one being used.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCameraSystem::OnActorLeave( class UPrimitiveComponent* InPrimitiveCompnent, AActor* InOtherActor, UPrimitiveComponent* InOtherPrimitiveComponent, int32 InInt )
{
	// If it is the player's camera dummy. [11/12/2015 Matthew Woolley]
	if ( InOtherActor->IsA( ARoadFeverCharacterNed::StaticClass() ) )
	{
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( InOtherActor );

		if ( PlayerCharacter )
		{
			// If this camera is currently being used. [13/4/2017 Matthew Woolley]
			if ( PlayerCharacter->CurrentCamera == this )
			{
				// If there is another camera area that influences the area the character is in. [13/4/2017 Matthew Woolley]
				if ( RevertTo != nullptr )
				{
					// Set that camera to be the one in use. [13/4/2017 Matthew Woolley]
					PlayerCharacter->CurrentCamera = RevertTo;
					PlayerCharacter->CharactersCamera->SetWorldLocation( RevertTo->CameraPosition.Location );
					PlayerCharacter->CharactersCamera->SetWorldRotation( RevertTo->CameraPosition.Rotation );
				}
				else
				{
					PlayerCharacter->CurrentCamera = nullptr;
				}
			}
		}

		if ( RevertTo == this )
		{
			RevertTo = nullptr;
		}
	}
}
