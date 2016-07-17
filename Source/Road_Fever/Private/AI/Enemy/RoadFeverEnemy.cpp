// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverEnemy.h"



// Setup default values for this enemy. [15/7/2016 Matthew Woolley]
ARoadFeverEnemy::ARoadFeverEnemy()
{
	EnemyHealth = 100;

	// Apply a placeholder mesh to the enemy. [15/7/2016 Matthew Woolley]
	const static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh( TEXT( "SkeletalMesh'/Game/Ned/Mesh/ProtoCharacters/SK_PrototypeM.SK_PrototypeM'" ) );
	GetMesh()->SetSkeletalMesh( EnemyMesh.Object );
	GetMesh()->SetRelativeLocation( FVector( 0, 0, -90 ) );
	GetMesh()->SetRelativeRotation( FRotator( 0, -90, 0 ) );

	HealthVisual = CreateDefaultSubobject<UTextRenderComponent>( TEXT( "Visual Text Render" ) );
	HealthVisual->SetText( FString::FromInt( EnemyHealth ) );
	HealthVisual->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	HealthVisual->SetRelativeLocation( FVector( 0, 0, 100 ) );
	HealthVisual->AttachParent = RootComponent;

	DistanceVisual = CreateDefaultSubobject<UTextRenderComponent>( TEXT( "Distance Text Render" ) );
	DistanceVisual->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	DistanceVisual->SetRelativeLocation( FVector( 0, 0, 120 ) );
	DistanceVisual->AttachParent = RootComponent;

	PrimaryActorTick.bCanEverTick = true;
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
	HealthVisual->SetText( FString::FromInt( EnemyHealth ) );

	if ( EnemyHealth <= 0 )
	{
		Die();
	}
}

void ARoadFeverEnemy::Tick( float DeltaSeconds )
{
	DistanceVisual->SetText( FString::SanitizeFloat( GetDistanceTo( GetWorld()->GetFirstPlayerController()->GetPawn() ) ) );
}
