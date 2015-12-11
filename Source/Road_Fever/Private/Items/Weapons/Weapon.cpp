// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Items/Weapons/Weapon.h"

AWeapon::AWeapon()
{
	// Default weapon properties [11/20/2015 Matthew Woolley]
	WeaponProperties.bIsCoolingDown = false;
	WeaponProperties.CoolDownTime = 1;
	WeaponProperties.Damage = 25;
	WeaponProperties.Range = 50;

	// Allow Actor ticking [11/20/2015 Matthew Woolley]
	PrimaryActorTick.bCanEverTick = true;

}

AWeapon::~AWeapon()
{

}

void AWeapon::Tick(float DeltaTime)
{
	// Time keeping variable [11/20/2015 Matthew Woolley]
	static float CurrentCoolDownTime = 0.f;

	// If the weapon needs to wait before being used again [11/20/2015 Matthew Woolley]
	if (WeaponProperties.bIsCoolingDown)
	{
		// Add up how long we have been waiting [11/20/2015 Matthew Woolley]
		CurrentCoolDownTime += DeltaTime;

		// If the attack has cooled down (i.e. can be used again) [11/20/2015 Matthew Woolley]
		if (CurrentCoolDownTime >= WeaponProperties.CoolDownTime)
		{
			// Reset the cool down timer [11/20/2015 Matthew Woolley]
			CurrentCoolDownTime = 0;

			// Tell the weapon it can be used again [11/20/2015 Matthew Woolley]
			WeaponProperties.bIsCoolingDown = false;
		}
	}
}

void AWeapon::OnAttack()
{
	// Make sure the weapon isn't currently cooling down [11/20/2015 Matthew Woolley]
	if (WeaponProperties.bIsCoolingDown)
	{
		return;
	}

	// The object found during the trace [11/20/2015 Matthew Woolley]
	FHitResult OutHit;

	// The current location of this weapon [11/20/2015 Matthew Woolley]
	FVector Start = GetActorLocation();

	// Get the furthest this weapon can attack [11/20/2015 Matthew Woolley]
	FVector End = Start + (GetActorRotation().Vector() * WeaponProperties.Range);

	// The rotation [11/20/2015 Matthew Woolley]
	FQuat Rot;

	// The trace's shape [11/20/2015 Matthew Woolley]
	FCollisionShape Shape;
	Shape.ShapeType = ECollisionShape::Capsule;
	Shape.MakeCapsule(20, 1); 

	// The parameters for the collision [11/20/2015 Matthew Woolley]
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.TraceTag = FName("WeaponTrace");

	// The current UWorld object [11/20/2015 Matthew Woolley]
	UWorld* const World = GetWorld();

	if (World)
	{
		// Trace to see if there are any Actors ahead [11/20/2015 Matthew Woolley]
		World->DebugDrawTraceTag = FName("WeaponTrace");							// TODO: WEAPON_TRACE ERROR
		bool bHadBlockingHit = World->SweepSingleByChannel(OutHit, Start, End, Rot, WEAPON_TRACE, Shape, Params);

		// If there was one [11/20/2015 Matthew Woolley]
		if (bHadBlockingHit)
		{
			// Make sure it's still valid and not being destroyed [11/20/2015 Matthew Woolley]
			AActor* HitActor = OutHit.GetActor();
			if (HitActor && !HitActor->IsPendingKill())
			{
				// Destroy it (WILL APPLY DAMAGE LATER ON) [11/20/2015 Matthew Woolley]
				HitActor->Destroy();
			}
		}
	}

	// Make sure the weapon cools down before shooting again [11/20/2015 Matthew Woolley]
	WeaponProperties.bIsCoolingDown = true;
}


