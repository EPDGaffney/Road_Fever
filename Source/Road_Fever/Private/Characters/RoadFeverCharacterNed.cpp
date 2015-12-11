// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCharacterNed.h"
#include "Kismet/KismetMathLibrary.h"



// Called when this Character enters memory. [10/12/2015 Matthew Woolley]
ARoadFeverCharacterNed::ARoadFeverCharacterNed()
{
	// Create the collection area. [10/12/2015 Matthew Woolley]
	CollectionArea = CreateDefaultSubobject<UBoxComponent>( TEXT( "CollectionArea" ) );
	CollectionArea->AttachParent = RootComponent;
	CollectionArea->RelativeScale3D = FVector( 2, 2, 2 );
	CollectionArea->RelativeLocation = FVector( 50, 0, 0 );

	// Setup the Character's movement. [11/12/2015 Matthew Woolley]
	BaseMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CharacterSprintSpeed = 1000;

	bIsDoingQuickTurn = false;
	QuickTurnSpeed = 5;
	QuickTurnWaitTime = 0.5f;
	TurnSensitivity = 2;

	// Character aiming. [11/12/2015 Matthew Woolley]
	AutoAimMaxDistance = 500.f;
	AutoAimSphere = CreateDefaultSubobject<USphereComponent>( TEXT( "AutoAimSphere" ) );
	AutoAimSphere->AttachParent = GetCapsuleComponent();
	AutoAimSphere->SetSphereRadius( AutoAimMaxDistance );

	// Allow Actor ticking. [11/12/2015 Matthew Woolley]
	PrimaryActorTick.bCanEverTick = true;
}

// Called at the beginning of game-play. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::BeginPlay()
{

}

// Called every frame. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::Tick( float InDeltaSeconds )
{
	// Call the initial implementation. [10/12/2015 Matthew Woolley]
	Super::Tick( InDeltaSeconds );

	// The amount of time since the quick-turn started. [10/12/2015 Matthew Woolley]
	static float QuickTurn_CurrentWaitTime = 0;
	
	// The Character's current yaw. [10/12/2015 Matthew Woolley]
	static float CharacterYaw = GetControlRotation().Yaw;

	// If we are quick-turning. [10/12/2015 Matthew Woolley]
	if ( bIsDoingQuickTurn )
	{
		// Add on the amount of time that has passed since the last time check. [10/12/2015 Matthew Woolley]
		QuickTurn_CurrentWaitTime += InDeltaSeconds;

		// Slowly rotate the character around. [10/12/2015 Matthew Woolley]
		GetController()->SetControlRotation( FMath::Lerp( GetControlRotation(), FRotator( 0, CharacterYaw + 180, 0 ), QuickTurnSpeed * InDeltaSeconds ) );

		// If the turn has finished. [10/12/2015 Matthew Woolley]
		if ( QuickTurn_CurrentWaitTime >= QuickTurnWaitTime )
		{
			// Tell the Character they are no longer quick-turning. [10/12/2015 Matthew Woolley]
			bIsDoingQuickTurn = false;
		}
	} else
	{
		// Update the CharacterYaw variable so it can be used as a reference later. [10/12/2015 Matthew Woolley]
		CharacterYaw = GetControlRotation().Yaw;
		// Reset the timer for the next time it is used [8/9/2015 Matthew Woolley]
		QuickTurn_CurrentWaitTime = 0;
	}
}

// Called to bind player input. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::SetupPlayerInputComponent( class UInputComponent* InInputComponent )
{
	Super::SetupPlayerInputComponent( InInputComponent );

	if ( InInputComponent )
	{
		InInputComponent->BindAxis( "MoveForward", this, &ARoadFeverCharacterNed::MoveForward );
		InInputComponent->BindAxis( "Turn", this, &ARoadFeverCharacterNed::Turn );

		InInputComponent->BindAction( "Action", IE_Pressed, this, &ARoadFeverCharacterNed::OnCharacterInteract );
		InInputComponent->BindAction( "Sprint", IE_Pressed, this, &ARoadFeverCharacterNed::OnBeginSprint );
		InInputComponent->BindAction( "Sprint", IE_Released, this, &ARoadFeverCharacterNed::OnEndSprint );

		InInputComponent->BindAction( "TurnAround", IE_Pressed, this, &ARoadFeverCharacterNed::OnBeginQuickTurn );

		InInputComponent->BindAction( "Aim", IE_Pressed, this, &ARoadFeverCharacterNed::OnBeginAim );
		InInputComponent->BindAction( "Aim", IE_Released, this, &ARoadFeverCharacterNed::OnEndAim );
	}
}

// Moves the Character in the X axis. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::MoveForward( float InInputVal )
{
	_move( InInputVal, EAxis::X );
}

// Called when the player wishes to turn. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::Turn( float InInputVal )
{
	AddControllerYawInput( InInputVal );
}

// Called when the player attempts to interact. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnCharacterInteract_Implementation()
{

}

// Called when the player wishes to sprint. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnBeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = CharacterSprintSpeed;
	bIsSprinting = true;
}

// Called when the player stops sprinting. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnEndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	bIsSprinting = false;
}

// Called when the player wishes to do a quick-turn. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnBeginQuickTurn()
{
	if ( !bIsDoingQuickTurn && MoveForwardAxis < 0 )
	{
		bIsDoingQuickTurn = true;
	}
}

// Called when the player begins aiming. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnBeginAim()
{

}

// Called when the player stops aiming. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnEndAim()
{

}

void ARoadFeverCharacterNed::_move( float InInputVal, EAxis::Type InMoveAxis )
{
	// Get the current rotation of the controller. [10/12/2015 Matthew Woolley]
	FRotator Rotation = GetControlRotation();

	// If we are falling or we are moving on the ground. [10/12/2015 Matthew Woolley]
	if ( GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsMovingOnGround() )
	{
		// Ignore the Character's pitch. [10/12/2015 Matthew Woolley]
		Rotation.Pitch = 0.f;
	}

	// Find the direction to move the Character in. [10/12/2015 Matthew Woolley]
	FVector MovementVector = FRotationMatrix( Rotation ).GetScaledAxis( InMoveAxis );
	// Move the Character in the aforementioned direction. [10/12/2015 Matthew Woolley]
	AddMovementInput( MovementVector, InInputVal );
}
