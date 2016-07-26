// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Items/Weapons/Weapon.h"
#include "Public/AI/Enemy/RoadFeverEnemy.h"
#include "Public/Characters/RoadFeverCharacterNed.h"



AWeapon::AWeapon()
{
	// Default weapon properties [20/11/2015 Matthew Woolley]
	WeaponProperties.bIsCoolingDown = false;
	WeaponProperties.CoolDownTime = 1;


	WeaponProperties.EffectiveRangeMaxDamage = 25;
	WeaponProperties.EffectiveRangeMinDamage = 20;

	WeaponProperties.MaximumRangeMaxDamage = 10;
	WeaponProperties.MaximumRangeMinDamage = 5;


	WeaponProperties.EffectiveRange = 50.f;
	WeaponProperties.MaximumRange = 100.f;

	WeaponProperties.NumberOfTraces = 1;
	WeaponProperties.MultiTraceSpread = 12.0f;

	// Make sure all weapons are treated as weapons. [26/7/2016 Matthew Woolley]
	ItemInfo.bIsWeapon = true;

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
	if ( WeaponProperties.bIsCoolingDown || ( ItemInfo.MaxAmmo != 0 && ItemInfo.CurrentAmmo == 0 ) )
	{
		return;
	}

	// For each trace to complete. [21/7/2016 Matthew Woolley]
	for ( int iTraceIterator = 0; iTraceIterator < WeaponProperties.NumberOfTraces; iTraceIterator++ )
	{
		// The object found during the trace [20/11/2015 Matthew Woolley]
		FHitResult OutHit;

		// The current location of this weapon [20/11/2015 Matthew Woolley]
		FVector Start = GetActorLocation();

		// The . [21/7/2016 Matthew Woolley]
		FRotator SpreadRotation = FRotator( FMath::FRandRange( WeaponProperties.MultiTraceSpread * -1, WeaponProperties.MultiTraceSpread ), FMath::FRandRange( WeaponProperties.MultiTraceSpread * -1, WeaponProperties.MultiTraceSpread ), 0 );

		// Get the furthest this weapon can attack [20/11/2015 Matthew Woolley]
		FVector End = Start + ( ( iTraceIterator == 0 ? GetActorRotation() : GetActorRotation() + SpreadRotation ).Vector() * WeaponProperties.MaximumRange );

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
						int DamageToDeal = FMath::FRandRange( WeaponProperties.EffectiveRangeMinDamage, WeaponProperties.EffectiveRangeMaxDamage ) - ( WeaponProperties.MaximumRangeMaxDamage * ( ( DistanceToEnemy - WeaponProperties.EffectiveRange ) / WeaponProperties.MaximumRange ) );

						// Deal damage to the enemy. [17/7/2016 Matthew Woolley]
						HitEnemy->TakeDamage( DamageToDeal );

					} else
					{
						// Deal damage to the enemy. [17/7/2016 Matthew Woolley]
						HitEnemy->TakeDamage( FMath::FRandRange( WeaponProperties.EffectiveRangeMinDamage, WeaponProperties.EffectiveRangeMaxDamage ) );
					}
				}
			}
		}

	}

	// Make sure the weapon cools down before shooting again [20/11/2015 Matthew Woolley]
	WeaponProperties.bIsCoolingDown = true;

	// If this weapon relies on ammo. [26/7/2016 Matthew Woolley]
	if ( ItemInfo.MaxAmmo != 0 )
	{
		// Remove ammo from the gun. [24/7/2016 Matthew Woolley]
		ItemInfo.CurrentAmmo -= 1;
	}
}

// Called when the user wishes to reload; bShouldUseFullClip will be true if they don't hold the reload key. [25/7/2016 Matthew Woolley]
bool AWeapon::Reload( bool bShouldUseFullClip )
{
	// Get Ned so we can use his inventory later. [25/7/2016 Matthew Woolley]
	ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );

	// If this weapon is fully out of ammo. [25/7/2016 Matthew Woolley]
	if ( ItemInfo.CurrentAmmo == 0 )
	{

	} else // If this weapon still contain ammo. [25/7/2016 Matthew Woolley]
	{
		UWorld* const World = GetWorld();

		if ( World )
		{
			// Try to add this as an item. [25/7/2016 Matthew Woolley]
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			AItem* TemporaryItemInfoHolder = World->SpawnActor<AItem>( ItemInfo.AmmoType, FVector( 0, 0, 0 ), GetActorRotation(), SpawnParams );

			if ( TemporaryItemInfoHolder )
			{
				GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT( "Spawned" ) );
				FInventoryItem ClipToAdd;
				ClipToAdd.bIsClip = true;
				ClipToAdd.bIsEquipable = false;
				ClipToAdd.CurrentAmmo = ItemInfo.CurrentAmmo;
				ClipToAdd.DisplayIcon = TemporaryItemInfoHolder->ItemInfo.DisplayIcon;
				ClipToAdd.DisplayName = TemporaryItemInfoHolder->ItemInfo.DisplayName;
				ClipToAdd.ItemClass = ItemInfo.ItemClass;
				ClipToAdd.ItemToolTip = TemporaryItemInfoHolder->ItemInfo.ItemToolTip;
				ClipToAdd.MaxItemStack = TemporaryItemInfoHolder->ItemInfo.MaxItemStack;

				bool bAddedItem = PlayerCharacter->AddItemToInventory( ClipToAdd );

				// If the clip wasn't added to the inventory. [25/7/2016 Matthew Woolley]
				if ( !bAddedItem )
				{
					// Throw the item onto the ground in front of Ned. [25/7/2016 Matthew Woolley]
					FVector SpawnLocation = PlayerCharacter->GetActorRotation().Vector() * 20;
					TemporaryItemInfoHolder->SetActorLocation( SpawnLocation );
				} else // If the item was added. [25/7/2016 Matthew Woolley]
				{
					// Destroy it from the level. [25/7/2016 Matthew Woolley]
					TemporaryItemInfoHolder->Destroy();
				}
			} else
			{
				GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT( "Failed to spawn" ) );
			}
		}
	}
	return true;
}

