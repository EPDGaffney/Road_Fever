// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCameraSystem.h"
#include "Public/Characters/RoadFeverCharacterNed.h"
#include "Public/Dummy Classes/RoadFeverCameraDummy.h"

// The CameraSystem to go to when leaving this one. [17/6/2016 Matthew Woolley]
ARoadFeverCameraSystem* RevertTo;

// Called when this Actor enters memory. [11/12/2015 Matthew Woolley]
ARoadFeverCameraSystem::ARoadFeverCameraSystem()
{
	// Create the class' root component. [11/12/2015 Matthew Woolley]
	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );

	// Create the trigger for this camera. [11/12/2015 Matthew Woolley]
	TriggerArea = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerArea" ) );
	TriggerArea->AttachParent = RootComponent;
	TriggerArea->bAbsoluteRotation = true;
	TriggerArea->SetCollisionProfileName( TEXT( "PlayerDetection" ) );
	TriggerArea->OnComponentBeginOverlap.AddDynamic( this, &ARoadFeverCameraSystem::OnActorEnter );
	TriggerArea->OnComponentEndOverlap.AddDynamic( this, &ARoadFeverCameraSystem::OnActorLeave );

	// Create the editor's camera. [11/12/2015 Matthew Woolley]
	EditorCameraReference = CreateDefaultSubobject<UCameraComponent>( TEXT( "EditorCameraReference" ) );
	EditorCameraReference->AttachParent = RootComponent;

	// Ticking setup. [20/1/2016 Matthew Woolley]
	PrimaryActorTick.bCanEverTick = true;
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

		// Set the camera's position. [11/12/2015 Matthew Woolley]
		PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
		PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
	} else
	{
		SetActorTickEnabled( false );
	}
}

// Called every frame. [20/1/2016 Matthew Woolley]
void ARoadFeverCameraSystem::Tick( float DeltaTime )
{
}

void ARoadFeverCameraSystem::OnActorEnter( class AActor* InOtherActor, class UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepResult )
{
	// If it is the player's camera dummy. [11/12/2015 Matthew Woolley]
	if ( InOtherActor->IsA( ARoadFeverCameraDummy::StaticClass() ) )
	{
		// Get the player's Character. [11/12/2015 Matthew Woolley]
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

		// If there isn't a Camera in use. [20/1/2016 Matthew Woolley]
		if ( PlayerCharacter->CurrentCamera == nullptr )
		{
			// Set the camera's position. [11/12/2015 Matthew Woolley]
			PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
			PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
			PlayerCharacter->CurrentCamera = this;
			RevertTo = nullptr;
		} else
		{
			RevertTo = PlayerCharacter->CurrentCamera;

			// Set the camera's position. [11/12/2015 Matthew Woolley]
			PlayerCharacter->CharactersCamera->SetWorldLocation( CameraPosition.Location );
			PlayerCharacter->CharactersCamera->SetWorldRotation( CameraPosition.Rotation );
			PlayerCharacter->CurrentCamera = this;
		}
	}
}

void ARoadFeverCameraSystem::OnActorLeave( class AActor* InOtherActor, class UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex )
{
	// If it is the player's camera dummy. [11/12/2015 Matthew Woolley]
	if ( InOtherActor->IsA( ARoadFeverCameraDummy::StaticClass() ) )
	{
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

		if ( PlayerCharacter->CurrentCamera == this )
		{
			if ( RevertTo != nullptr )
			{
				PlayerCharacter->CurrentCamera = RevertTo;

				// Set the camera's position. [11/12/2015 Matthew Woolley]
				PlayerCharacter->CharactersCamera->SetWorldLocation( RevertTo->CameraPosition.Location );
				PlayerCharacter->CharactersCamera->SetWorldRotation( RevertTo->CameraPosition.Rotation );
			} else
			{
				PlayerCharacter->CurrentCamera = nullptr;
			}
		}

		if ( RevertTo == this )
		{
			RevertTo = nullptr;
		}

		SetActorTickEnabled( false );
	}
}
