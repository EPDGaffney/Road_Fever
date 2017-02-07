// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverEnemy.h"



// Setup default values for this enemy. [15/7/2016 Matthew Woolley]
ARoadFeverEnemy::ARoadFeverEnemy()
{
	EnemyHealth = 100;
	MinMoveSpeed = 1;
	MaxMoveSpeed = 1;

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
		GetMesh()->SetSkeletalMesh( EnemyMeshes[EnemyMeshToUse] );
	}

	Super::BeginPlay();
}

// Called when this enemy no longer has any health. [15/7/2016 Matthew Woolley]
void ARoadFeverEnemy::Die_Implementation()
{
	// If this enemy isn't already being killed. [15/7/2016 Matthew Woolley]
	if ( !IsPendingKill() && EnemyHealth <= 0 )
	{
		// Kill it. [15/7/2016 Matthew Woolley]
		Destroy();
	}
}

// Takes away health from this enemy. [15/7/2016 Matthew Woolley]
void ARoadFeverEnemy::TakeDamage_Implementation( int32 InDamage )
{
	EnemyHealth -= InDamage;

	if ( EnemyHealth <= 0 )
	{
		Die();
	}
}