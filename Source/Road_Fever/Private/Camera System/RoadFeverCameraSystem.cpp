// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCameraSystem.h"
#include "Public/Characters/RoadFeverCharacterNed.h"
#include "Public/Dummy Classes/RoadFeverCameraDummy.h"


// Called when this Actor enters memory. [11/12/2015 Matthew Woolley]
ARoadFeverCameraSystem::ARoadFeverCameraSystem()
{
	// Create the class' root component. [11/12/2015 Matthew Woolley]
	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );

	// Create the trigger for this camera. [11/12/2015 Matthew Woolley]
	TriggerArea = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerArea" ) );
	TriggerArea->AttachParent = RootComponent;
	TriggerArea->bAbsoluteRotation = true;
	TriggerArea->OnComponentBeginOverlap.AddDynamic( this, &ARoadFeverCameraSystem::OnActorEnter );

	// Create the editor's camera. [11/12/2015 Matthew Woolley]
	EditorCameraReference = CreateDefaultSubobject<UCameraComponent>( TEXT( "EditorCameraReference" ) );
	EditorCameraReference->AttachParent = RootComponent;
}


// Called at the beginning of game-play. [11/12/2015 Matthew Woolley]
void ARoadFeverCameraSystem::BeginPlay()
{
	Super::BeginPlay();

	// Get the camera's position. [11/12/2015 Matthew Woolley]
	CameraPosition.Location = EditorCameraReference->GetComponentLocation();
	CameraPosition.Rotation = EditorCameraReference->GetComponentRotation();

	// Destroy the camera so that the game doesn't keep rendering un-needed scenes. [11/12/2015 Matthew Woolley]
	EditorCameraReference->DestroyComponent();

	if ( bIsPrimaryCamera )
	{
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );
		OnActorEnter( PlayerCharacter->GetCameraDummy(), nullptr, 0, false, FHitResult() );
	}
}

void ARoadFeverCameraSystem::OnActorEnter( class AActor* InOtherActor, class UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult )
{
	// If it is the player's camera dummy. [11/12/2015 Matthew Woolley]
	if ( InOtherActor->IsA( ARoadFeverCameraDummy::StaticClass() ) )
	{
		// Get the player's Character. [11/12/2015 Matthew Woolley]
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

		// Set the camera's position. [11/12/2015 Matthew Woolley]
		PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
		PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
	}
}
