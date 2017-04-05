// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverEnemy.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



// Setup default values for this enemy. [15/7/2016 Matthew Woolley]
ARoadFeverEnemy::ARoadFeverEnemy()
{
	// Set-up the enemy's stats. [8/2/2017 Matthew Woolley]
	EnemyHealth = 100;
	MinMoveSpeed = 1;
	MaxMoveSpeed = 1;

	// Set-up the mesh so it isn't orientated strangely. [8/2/2017 Matthew Woolley]
	GetMesh()->SetRelativeLocation( FVector( 0, 0, -90 ) );
	GetMesh()->SetRelativeRotation( FRotator( 0, -90, 0 ) );
}

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

// Called when this enemy no longer has any health. [15/7/2016 Matthew Woolley]
void ARoadFeverEnemy::Die_Implementation()
{
	// No default implementation of this method. [8/2/2017 Matthew Woolley]
}

// Takes away health from this enemy. [15/7/2016 Matthew Woolley]
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

// Whether the player is in the inventory, or item confirmation screen. [17/3/2017 Matthew Woolley]
const bool ARoadFeverEnemy::IsPlayerPaused()
{
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		// Get Ned's character from the world. [17/3/2017 Matthew Woolley]
		ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

		// If we got Ned from the level. [17/3/2017 Matthew Woolley]
		if ( PlayerCharacter )
		{
			// Return whether the player has the game in focus or not. [17/3/2017 Matthew Woolley]
			return !PlayerCharacter->GameHasFocus();
		}
	}

	// If we didn't find Ned. [17/3/2017 Matthew Woolley]
	return false;
}
