// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverEnemy.h"



// Setup default values for this enemy. [15/7/2016 Matthew Woolley]
ARoadFeverEnemy::ARoadFeverEnemy()
{
	EnemyHealth = 100;
	MinMoveSpeed = 1;
	MaxMoveSpeed = 1;

	// Apply a placeholder mesh to the enemy. [15/7/2016 Matthew Woolley]
	const static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh( TEXT( "SkeletalMesh'/Game/Ned/Mesh/ProtoCharacters/SK_PrototypeM.SK_PrototypeM'" ) );
	GetMesh()->SetSkeletalMesh( EnemyMesh.Object );
	GetMesh()->SetRelativeLocation( FVector( 0, 0, -90 ) );
	GetMesh()->SetRelativeRotation( FRotator( 0, -90, 0 ) );
}

void ARoadFeverEnemy::BeginPlay()
{
	MoveSpeed = FMath::FRandRange( MinMoveSpeed, MaxMoveSpeed );
}

// Called when this enemy no longer has any health. [15/7/2016 Matthew Woolley]
void ARoadFeverEnemy::Die()
{
	// If this enemy isn't already being killed. [15/7/2016 Matthew Woolley]
	if ( !IsPendingKill() && EnemyHealth <= 0 )
	{
		// Kill it. [15/7/2016 Matthew Woolley]
		Destroy();
	}
}

// Takes away health from this enemy. [15/7/2016 Matthew Woolley]
void ARoadFeverEnemy::TakeDamage( int32 InDamage )
{
	EnemyHealth -= InDamage;

	if ( EnemyHealth <= 0 )
	{
		Die();
	}
}