// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverEnemy.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



/*
 *	Gives the enemy its default health and speed.
 *	Sets up the location and rotation for the mesh so that it doesn't need to be fiddled with at run-time when the mesh is assigned.
 *	11/4/2017 - Matthew Woolley
 */
ARoadFeverEnemy::ARoadFeverEnemy()
{
	// Set-up the enemy's stats. [8/2/2017 Matthew Woolley]
	EnemyHealth = 100;
	MinMoveSpeed = 1;
	MaxMoveSpeed = 1;

	// Set-up the mesh so it isn't orientated strangely. [8/2/2017 Matthew Woolley]
	GetMesh()->SetRelativeLocation( FVector( 0, 0, -90 ) );
	GetMesh()->SetRelativeRotation( FRotator( 0, -90, 0 ) );

	GetCapsuleComponent()->SetCollisionProfileName( "Enemy" );
}

/*
 *	Get a random speed for the enemy to move at, so that they don't all look like they are walking in-step.
 *	Get a random mesh from a pool of meshes so that not all enemies look the same, even when they are the same type.
 *	Call the Super:: so that any BP implemented version of this works.
 */
void ARoadFeverEnemy::BeginPlay()
{
	MoveSpeed = FMath::FRandRange( MinMoveSpeed, MaxMoveSpeed );

	// If there is an enemy in the random enemy pool. [3/2/2017 Matthew Woolley]
	if ( EnemyMeshes.Num() != 0 )
	{
		// Pick a random enemy mesh. [3/2/2017 Matthew Woolley]
		int EnemyMeshToUse = FMath::FRandRange( 0, EnemyMeshes.Num() );

		// Set it as the mesh for this instance. [3/2/2017 Matthew Woolley]
		GetMesh()->SetSkeletalMesh( EnemyMeshes[ EnemyMeshToUse ] );
	}

	Super::BeginPlay();
}

/*
 *	Called when this enemy has taken damage and has no remaining health.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverEnemy::Die_Implementation()
{
	// No default implementation of this method. [8/2/2017 Matthew Woolley]
}

/*
 *	Call BP functionality (Spawns effects).
 *	Take the amount of damage dealt away from the enemy's health.
 *	Figure out whether the enemy has died or not.
 *	Return the amout of health remaining of the enemy.
 *	11/4/2017 - Matthew Woolley
 */
float ARoadFeverEnemy::TakeDamage( float InDamage, struct FDamageEvent const& InDamageEvent, class AController* InEventInstigator, class AActor* InDamageCauser )
{
	// Call base functionality [23/3/2017 Matthew Woolley]
	Super::TakeDamage( InDamage, InDamageEvent, InEventInstigator, InDamageCauser );

	// Deal damage to the enemy. [8/2/2017 Matthew Woolley]
	EnemyHealth -= InDamage;

	// Check if the enemy has died. [8/2/2017 Matthew Woolley]
	if ( EnemyHealth <= 0 )
	{
		Die();
	}

	// Return the value of this enemy's health. [23/3/2017 Matthew Woolley]
	return EnemyHealth;
}

/*
 *	Returns whether or not the player is playing (false) or in a menu (true).
 *	11/4/2017 - Matthew Woolley
 */
const bool ARoadFeverEnemy::IsPlayerPaused()
{
	// Find out whether the game world exists and whether the controller exists. [11/4/2017 Matthew Woolley]
	if ( GetWorld() && GetWorld()->GetFirstPlayerController() )
	{
		// Get Ned's character from the world. [17/3/2017 Matthew Woolley]
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

		// If the Ned exists. [11/4/2017 Matthew Woolley]
		if ( PlayerCharacter )
		{
			// Return whether the player has the game in focus or not. [17/3/2017 Matthew Woolley]
			return !PlayerCharacter->GameHasFocus();
		}
	}

	// If we didn't find Ned. [17/3/2017 Matthew Woolley]
	return false;
}
