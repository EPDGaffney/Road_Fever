// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Items/Weapons/Weapon.h"
#include "Public/AI/Enemy/RoadFeverEnemy.h"



AWeapon::AWeapon()
{
	// Default weapon properties [20/11/2015 Matthew Woolley]
	WeaponProperties.bIsCoolingDown = false;
	WeaponProperties.CoolDownTime = 1;
	WeaponProperties.MaxDamage = 25;
	WeaponProperties.MaximumRangeDamageDropoff = 10;
	WeaponProperties.EffectiveRange = 50.f;
	WeaponProperties.MaximumRange = 100.f;

	// Allow Actor ticking [20/11/2015 Matthew Woolley]
	PrimaryActorTick.bCanEverTick = true;

}

AWeapon::~AWeapon()
{

}

void AWeapon::Tick( float DeltaTime )
{
	// Time keeping variable [20/11/2015 Matthew Woolley]
	static float CurrentCoolDownTime = 0.f;

	// If the weapon needs to wait before being used again [20/11/2015 Matthew Woolley]
	if ( WeaponProperties.bIsCoolingDown )
	{
		// Add up how long we have been waiting [20/11/2015 Matthew Woolley]
		CurrentCoolDownTime += DeltaTime;

		// If the attack has cooled down (i.e. can be used again) [20/11/2015 Matthew Woolley]
		if ( CurrentCoolDownTime >= WeaponProperties.CoolDownTime )
		{
			// Reset the cool down timer [20/11/2015 Matthew Woolley]
			CurrentCoolDownTime = 0;

			// Tell the weapon it can be used again [20/11/2015 Matthew Woolley]
			WeaponProperties.bIsCoolingDown = false;
		}
	}
}


// Called when the player wishes to attack with this weapon [20/11/2015 Matthew Woolley]
void AWeapon::OnAttack_Implementation()
{
	// Make sure the weapon isn't currently cooling down [20/11/2015 Matthew Woolley]
	if ( WeaponProperties.bIsCoolingDown )
	{
		return;
	}

	// The object found during the trace [20/11/2015 Matthew Woolley]
	FHitResult OutHit;

	// The current location of this weapon [20/11/2015 Matthew Woolley]
	FVector Start = GetActorLocation();

	// Get the furthest this weapon can attack [20/11/2015 Matthew Woolley]
	FVector End = Start + ( GetActorRotation().Vector() * WeaponProperties.MaximumRange );

	// The rotation [20/11/2015 Matthew Woolley]
	FQuat Rot;

	// The trace's shape [20/11/2015 Matthew Woolley]
	FCollisionShape Shape;
	Shape.ShapeType = ECollisionShape::Capsule;
	Shape.MakeCapsule( 20, 1 );

	// The parameters for the collision [20/11/2015 Matthew Woolley]
	FCollisionQueryParams Params;
	Params.AddIgnoredActor( this );
	Params.AddIgnoredActor( GetWorld()->GetFirstPlayerController()->GetPawn() );
	Params.TraceTag = FName( "WeaponTrace" );

	// The current UWorld object [20/11/2015 Matthew Woolley]
	UWorld* const World = GetWorld();

	FCollisionResponseParams RespParams;
	RespParams.CollisionResponse.Visibility;

	if ( World )
	{
		// Trace to see if there are any Actors ahead [20/11/2015 Matthew Woolley]
		World->DebugDrawTraceTag = FName( "WeaponTrace" );
		bool bHadBlockingHit = World->SweepSingleByChannel( OutHit, Start, End, Rot, WEAPON_TRACE, Shape, Params, RespParams );

		// If there was one [20/11/2015 Matthew Woolley]
		if ( bHadBlockingHit )
		{
			// Make sure it's still valid and not being destroyed [20/11/2015 Matthew Woolley]
			AActor* HitActor = OutHit.GetActor();
			if ( HitActor && !HitActor->IsPendingKill() && HitActor->IsA( ARoadFeverEnemy::StaticClass() ) )
			{
				// Cast the enemy from the hit Actor. [15/7/2016 Matthew Woolley]
				ARoadFeverEnemy* HitEnemy = ( ARoadFeverEnemy* ) HitActor;

				// If the enemy is further away than this weapon can attack with full damage. [17/7/2016 Matthew Woolley]
				if ( HitEnemy->GetDistanceTo( this ) > WeaponProperties.EffectiveRange )
				{
					// Get the distance to the enemy being attacked. [17/7/2016 Matthew Woolley]
					float DistanceToEnemy = HitEnemy->GetDistanceTo( this );

					// Get the damage we should deal (the closer to the maximum range distance, the more damage). [17/7/2016 Matthew Woolley]
					int DamageToDeal = WeaponProperties.MaxDamage - ( WeaponProperties.MaximumRangeDamageDropoff * ( ( DistanceToEnemy - WeaponProperties.EffectiveRange ) / WeaponProperties.MaximumRange ) );

					// Deal damage to the enemy. [17/7/2016 Matthew Woolley]
					HitEnemy->TakeDamage( DamageToDeal );

				} else
				{
					// Deal damage to the enemy. [17/7/2016 Matthew Woolley]
					HitEnemy->TakeDamage( WeaponProperties.MaxDamage );
				}
			}
		}
	}

	// Make sure the weapon cools down before shooting again [20/11/2015 Matthew Woolley]
	WeaponProperties.bIsCoolingDown = true;
}

