// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCharacterNed.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Inventory/Inventory.h"
#include "Public/Items/Weapons/Weapon.h"
#include "Public/AI/Enemy/RoadFeverEnemy.h"



/*
 *	Sets up the collection area for collecting items.
 *	Sets up the shooting point for weapons' ray tracing.
 *	Sets up the default settings like sensitivity.
 *	Creates the camera that is used by the CameraSystem.
 *	Creates the inventory for later use.
 *	Sets up default stats like Health and BloodLoss.
 *	Turns ticking on.
 *	11/4/2017 - Matthew Woolley
 */
ARoadFeverCharacterNed::ARoadFeverCharacterNed()
{
	// Create the collection area. [10/12/2015 Matthew Woolley]
	CollectionArea = CreateDefaultSubobject<UBoxComponent>( TEXT( "CollectionArea" ) );
	CollectionArea->SetupAttachment( RootComponent );
	CollectionArea->RelativeScale3D = FVector( 2, 2, 2 );
	CollectionArea->RelativeLocation = FVector( 50, 0, 0 );

	// Create the ShootFromPoint. [1/3/2017 Matthew Woolley]
	ShootFromPoint = CreateDefaultSubobject<UArrowComponent>( TEXT( "ShootFromPoint" ) );
	ShootFromPoint->SetupAttachment(RootComponent);
	ShootFromPoint->RelativeLocation = FVector( 10, 10, 50 );

	QuickTurnSpeed = 500;
	TurnSensitivity = 2;

	bIsDebugging = false;

	// Character's camera component. [11/12/2015 Matthew Woolley]
	CharactersCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "NedCamera" ) );
	CharactersCamera->bAbsoluteLocation = true;
	CharactersCamera->bAbsoluteRotation = true;

	// Character's inventory component. [12/12/2015 Matthew Woolley]
	CharactersInventory = CreateDefaultSubobject<UInventory>( TEXT( "Character's Inventory" ) );

	// Setup the player's stats. [25/12/2015 Matthew Woolley]
	Health = 1000;
	BloodLoss = 1000;

	// Allow Actor ticking. [11/12/2015 Matthew Woolley]
	PrimaryActorTick.bCanEverTick = true;
}

/*
 *	Switches animations based on whether anything is equipped or whether the player is aiming.
 *	Turns the character if the DegreesToTurn is not 0 or less (used for quick turning).
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::Tick( float InDeltaSeconds )
{
	// Call the initial implementation. [10/12/2015 Matthew Woolley]
	Super::Tick( InDeltaSeconds );

	// If the player has an equipped item. [18/7/2016 Matthew Woolley]
	if ( CharactersInventory->EquippedItem != nullptr )
	{
		// If the the player is aiming. [18/7/2016 Matthew Woolley]
		if ( bIsAiming )
		{
			// Use the aiming Blueprint. [18/7/2016 Matthew Woolley]
			GetMesh()->SetAnimInstanceClass( CharactersInventory->EquippedItem->AnimationBlueprint_Aiming );
		} else
		{
			// If the player isn't aiming. [19/7/2016 Matthew Woolley]
			GetMesh()->SetAnimInstanceClass( CharactersInventory->EquippedItem->AnimationBlueprint_Default );
		}
	} else
	{
		// If the player hasn't got a weapon equipped. [19/7/2016 Matthew Woolley]
		GetMesh()->SetAnimInstanceClass( DefaultAnimation );
	}

	// If we are doing a quick turn. [18/7/2016 Matthew Woolley]
	if ( DegreesToTurn > 0 )
	{
		// Stop the character's movement. [18/7/2016 Matthew Woolley]
		GetCharacterMovement()->SetMovementMode( EMovementMode::MOVE_None );

		// Take off how much we will turn. [18/7/2016 Matthew Woolley]
		DegreesToTurn -= InDeltaSeconds * QuickTurnSpeed;

		FRotator RotationToChange = GetControlRotation();
		RotationToChange.Yaw += InDeltaSeconds * QuickTurnSpeed;

		GetController()->SetControlRotation( FMath::RInterpTo( GetControlRotation(), RotationToChange, 3, 0 ) );
	} else
	{
		// Re-enable the character's movement. [18/7/2016 Matthew Woolley]
		GetCharacterMovement()->SetMovementMode( EMovementMode::MOVE_Walking );
	}
}

/*
 *	Sets up the default input bindings.
 *	11/4/2017 - Matthew Woolley
 */
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
		InInputComponent->BindAxis( "Aim", this, &ARoadFeverCharacterNed::AimUp_Down );
		InInputComponent->BindAction( "Attack", IE_Pressed, this, &ARoadFeverCharacterNed::OnAttack );

		InInputComponent->BindAction( "ToggleInventory", IE_Pressed, CharactersInventory, &UInventory::ToggleInventory );

		InInputComponent->BindAction( "SwitchEnemy", IE_Pressed, this, &ARoadFeverCharacterNed::SwitchToNextEnemy );

		InInputComponent->BindAxis( "Reload", this, &ARoadFeverCharacterNed::Reload );
	}
}

/*
 *	Returns a string that is used by the UI to display the current state of the player's health.
 *	11/4/2017 - Matthew Woolley
 */
FString ARoadFeverCharacterNed::UpdateHealthMessage()
{
	// If the player has more than 75% of their health. [25/12/2015 Matthew Woolley]
	if ( Health > 750 )
	{
		return FString( "Strong" );
	} else if ( Health > 500 ) // If they have less than 75% and more than 50%. [25/12/2015 Matthew Woolley]
	{
		return FString( "Compromised" );
	} else if ( Health > 250 ) // If they have less than 50% and more than 25%. [25/12/2015 Matthew Woolley]
	{
		return FString( "Caution" );
	} else // If they have less than 25%. [25/12/2015 Matthew Woolley]
	{
		return FString( "Danger" );
	}
}

/*
*	Returns a string that is used by the UI to display the current state of the player's blood loss.
*	11/4/2017 - Matthew Woolley
*/
FString ARoadFeverCharacterNed::UpdateBloodMessage()
{
	// If the player has more than 75% of blood loss. [25/12/2015 Matthew Woolley]
	if ( BloodLoss > 750 )
	{
		return FString( "Minor" );
	} else if ( BloodLoss > 500 ) // If they have less than 75% and more than 50%. [25/12/2015 Matthew Woolley]
	{
		return FString( "Moderate" );
	} else if ( BloodLoss > 250 ) // If they have less than 50% and more than 25%. [25/12/2015 Matthew Woolley]
	{
		return FString( "Caution" );
	} else // If they have less than 25%. [25/12/2015 Matthew Woolley]
	{
		return FString( "Danger" );
	}
}

/*
 *	Finds a stack that the item being added can be added into (true).
 *	Finds an empty slot that the item being added can be added into (true).
 *	Leaves the item if there was no space for it (false).
 *	11/4/2017 - Matthew Woolley
 */
bool ARoadFeverCharacterNed::AddItemToInventory( struct FInventoryItem ItemToAdd )
{
	// For each inventory slot. [5/3/2016 Matthew Woolley]
	for ( int32 iSlotIterator = 0; iSlotIterator < CharactersInventory->ItemSlots.Num(); iSlotIterator++ )
	{
		// If the item that is trying to be added is the same one that is already in this slot. [5/3/2016 Matthew Woolley]
		if ( CharactersInventory->ItemSlots[ iSlotIterator ].DisplayName == ItemToAdd.DisplayName )
		{
			// If there is less of the item than you can carry in one slot. [24/7/2016 Matthew Woolley]
			if ( CharactersInventory->ItemSlots[ iSlotIterator ].CurrentItemStack < ItemToAdd.MaxItemStack )
			{
				// If this is a clip and it has found a clip of similar size OR it isn't a clip. [25/7/2016 Matthew Woolley]
				if ( ( ItemToAdd.bClip && ItemToAdd.CurrentAmmo == CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo ) || !ItemToAdd.bClip )
				{
					// Add an item to the slot. [5/3/2016 Matthew Woolley]
					CharactersInventory->ItemSlots[ iSlotIterator ].CurrentItemStack++;

					// Tell the code that this item was added successfully. [5/3/2016 Matthew Woolley]
					return true;
				}
			} else // If there isn't enough space. [24/7/2016 Matthew Woolley]
			{
				continue;
			}
		}
	}

	// For each inventory slot (when there is not an available slot). [24/7/2016 Matthew Woolley]
	for ( int32 iSlotIterator = 0; iSlotIterator < CharactersInventory->ItemSlots.Num(); iSlotIterator++ )
	{
		// If it isn't the same item but is instead empty. [5/3/2016 Matthew Woolley]
		if ( CharactersInventory->ItemSlots[ iSlotIterator ].DisplayName == "" )
		{
			// Set this item up inside this slot. [5/3/2016 Matthew Woolley]
			CharactersInventory->ItemSlots[ iSlotIterator ].CurrentItemStack++;
			CharactersInventory->ItemSlots[ iSlotIterator ].DisplayIcon = ItemToAdd.DisplayIcon;
			CharactersInventory->ItemSlots[ iSlotIterator ].DisplayName = ItemToAdd.DisplayName;
			CharactersInventory->ItemSlots[ iSlotIterator ].ItemClass = ItemToAdd.ItemClass;
			CharactersInventory->ItemSlots[ iSlotIterator ].ItemToolTip = ItemToAdd.ItemToolTip;
			CharactersInventory->ItemSlots[ iSlotIterator ].MaxItemStack = ItemToAdd.MaxItemStack;
			CharactersInventory->ItemSlots[ iSlotIterator ].bEquipable = ItemToAdd.bEquipable;
			CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = ItemToAdd.CurrentAmmo;

			// If this item is a weapon. [24/7/2016 Matthew Woolley]
			if ( ItemToAdd.bWeapon )
			{
				// Setup the weapon stats. [24/7/2016 Matthew Woolley]
				CharactersInventory->ItemSlots[ iSlotIterator ].MaxAmmo = ItemToAdd.MaxAmmo;
				CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = ItemToAdd.CurrentAmmo;
			} else
			{
				// Setup the weapon stats. [24/7/2016 Matthew Woolley]
				CharactersInventory->ItemSlots[ iSlotIterator ].MaxAmmo = 0;
				CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = 0;
			}

			// If this is a clip ammo type (other ammo types are added normally). [25/7/2016 Matthew Woolley]
			if ( ItemToAdd.bClip )
			{
				CharactersInventory->ItemSlots[ iSlotIterator ].bClip = ItemToAdd.bClip;
				CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = ItemToAdd.CurrentAmmo;
			}

			// Tell the code that this item was added successfully. [5/3/2016 Matthew Woolley]
			return true;
		}
	}

	// If the item wasn't added, tell the code that it failed. [5/3/2016 Matthew Woolley]
	return false;
}

/*
 *	Whether or not this player is in a menu, paused (true), or playing (false).
 *	11/4/2017 - Matthew Woolley
 */
const bool ARoadFeverCharacterNed::GameHasFocus()
{
	return ( !CharactersInventory->bOpen && !CharactersInventory->ItemPickupConfirmationInstance );
}

/*
 *	If the player isn't aiming, it assigns the MoveForwardAxis variable to be equal to that of the player input
 *	the AnimationBP then uses this for RootMotion and moves the character Forward (+) and backward (-).
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::MoveForward( float InInputVal )
{
	if ( bIsAiming == false && GameHasFocus() )
	{
		MoveForwardAxis = InInputVal;
	} else
	{
		MoveForwardAxis = 0;
	}
}

/*
 *	If the player is allowed to move, turn the character right (+) or left (-) with the sensitivity
 *	that has been set.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::Turn( float InInputVal )
{
	if ( !GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None )
	{
		AddControllerYawInput( ( GameHasFocus() ? InInputVal * ( TurnSensitivity * GetWorld()->GetDeltaSeconds() ) : 0 ) );
	}
}

/*
 *	If the game has focus, and the player is not aiming, interact with the nearest item.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnCharacterInteract_Implementation()
{
	if ( !GameHasFocus() || bIsAiming )
	{
		return;
	}

	// Make sure the collection area exists. [15/12/2015 Matthew Woolley]
	if ( CollectionArea )
	{
		// Get all actors that are overlapping the collection area. [15/12/2015 Matthew Woolley]
		TArray<AActor*> NearbyActors;
		CollectionArea->GetOverlappingActors( NearbyActors, AItem::StaticClass() );

		// Store all of th found AItems so their distance can be checked later on. [22/7/2016 Matthew Woolley]
		TArray<AItem*> NearbyItems;

		// Get each actor. [15/12/2015 Matthew Woolley]
		for ( AActor* iActorIterator : NearbyActors )
		{
			// If the actor we found is an AItem AND it can be picked up. [22/6/2016 Matthew Woolley]
			if ( iActorIterator->IsA( AItem::StaticClass() ) && ( ( AItem* ) iActorIterator )->bPickupable )
			{
				// Get reference to AItem found. [22/6/2016 Matthew Woolley]
				AItem* Item = ( AItem* ) iActorIterator;

				// Get the world. [15/12/2015 Matthew Woolley]
				UWorld* const World = GetWorld();

				// Collision parameters for line-trace. [15/12/2015 Matthew Woolley]
				FHitResult Hit;
				FCollisionQueryParams Line( FName( "CollisionParam" ), true );
				TArray<AActor*> IgnoredActors;
				IgnoredActors.Add( this );
				Line.AddIgnoredActors( IgnoredActors );

				// Find if there is anything blocking the pickup. [15/12/2015 Matthew Woolley]
				bool bHasBlockingHit = World->LineTraceSingleByChannel( Hit, this->GetActorLocation(), Item->GetActorLocation(), COLLISION_TRACE, Line );

				// If we hit something and it is the item. [15/12/2015 Matthew Woolley]
				if ( bHasBlockingHit && Hit.GetActor() == Item )
				{
					// Add it to the NearbyItem array so we can check its distance. [22/7/2016 Matthew Woolley]
					NearbyItems.Add( Item );
				}
			}
		}

		// The AItem and distance that are closest to this player. [22/7/2016 Matthew Woolley]
		float ItemBeingChecked = 1000.f;
		AItem* ClosestItem = nullptr;

		// For each found item. [22/7/2016 Matthew Woolley]
		for ( AItem* CurrentlyCheckedItem : NearbyItems )
		{
			// If it is closer than the last item. [22/7/2016 Matthew Woolley]
			if ( GetDistanceTo( CurrentlyCheckedItem ) <= ItemBeingChecked )
			{
				// Set it as the closest item. [22/7/2016 Matthew Woolley]
				ClosestItem = CurrentlyCheckedItem;
				ItemBeingChecked = GetDistanceTo( CurrentlyCheckedItem );
			}
		}

		// If we found an item. [22/7/2016 Matthew Woolley]
		if ( ClosestItem )
		{
			// Set the closest item as the item being interacted with AND pause the game. [22/7/2016 Matthew Woolley]
			ItemBeingInteractedWith = ClosestItem;
			APlayerController* const PlayerController = ( APlayerController* ) GEngine->GetFirstLocalPlayerController( GetWorld() );
			PlayerController->SetPause( true );
			CharactersInventory->OpenPickupConfirmation();
		}
	}
}

/*
 *	If the game isn't paused and the player isn't aiming, set Sprinting to true
 *	so that the AnimationBP can change the animation to a sprinting one.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnBeginSprint()
{
	if ( !GameHasFocus() || bIsAiming )
	{
		return;
	}

	bIsSprinting = true;
}

/*
 *	Set Sprinting to false so that the AnimationBP uses default walking animations.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnEndSprint()
{
	bIsSprinting = false;
}

/*
 *	Sets the DegreesToTurn to 180° so that the player turns around (done in Tick).
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnBeginQuickTurn()
{
	if ( !GameHasFocus() || bIsAiming )
	{
		return;
	}

	if ( DegreesToTurn <= 0 && MoveForwardAxis < 0 )
	{
		DegreesToTurn = 180;
	}
}

/*
 *	When the player begins aiming, find the nearest enemy and look at it.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnBeginAim()
{
	if ( GameHasFocus() && CharactersInventory->EquippedItem )
	{
		bIsAiming = true;

		// The enemies that can be aimed at. [17/7/2016 Matthew Woolley]
		TArray<AActor*> Enemies = GetEnemies();

		// If we have found an enemy. [17/7/2016 Matthew Woolley]
		if ( Enemies.Num() != 0 )
		{
			// Get the distance to the closest enemy found. [17/7/2016 Matthew Woolley]
			float SmallestDistance = -0;
			// Store the actual enemy so, if it is the closest one, we can look at it. [17/7/2016 Matthew Woolley]
			AActor* ClosestEnemy = nullptr;

			// For each enemy found. [17/7/2016 Matthew Woolley]
			for ( AActor* iRoadFeverEnemyIterator : Enemies )
			{
				// If this enemy is closer than the last one. [17/7/2016 Matthew Woolley]
				if ( iRoadFeverEnemyIterator->GetDistanceTo( this ) <= SmallestDistance || SmallestDistance == -0 )
				{
					// Store it so we can check its distance against the next enemy. [17/7/2016 Matthew Woolley]
					SmallestDistance = iRoadFeverEnemyIterator->GetDistanceTo( this );
					ClosestEnemy = iRoadFeverEnemyIterator;
				}
			}

			// If we have a closest enemy. [17/7/2016 Matthew Woolley]
			if ( ClosestEnemy )
			{
				// Make Ned look at the found enemy. [16/7/2016 Matthew Woolley]
				FRotator LookAtRotation = ( GetActorLocation() - ClosestEnemy->GetActorLocation() ).Rotation();
				GetController()->SetControlRotation( FRotator( 0, LookAtRotation.Yaw - 180, 0 ) );
			}
		}
	}
}

/*
 *	Used so that the animation can go back to not-aiming and so that the player can move again.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnEndAim()
{
	bIsAiming = false;
}

/*
 *	Fires a ray-trace from the weapon which then determines whether and enemy was hit.
 *	Logic for shooting is done within the Weapon class.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::OnAttack()
{
	// Ensure the player is playing the game (not in a menu, for example), aiming, and has a weapon equipped. [13/4/2017 Matthew Woolley]
	if ( !GameHasFocus() || !bIsAiming || !CharactersInventory->EquippedItem && !( AWeapon* ) CharactersInventory->EquippedItem )
	{
		return;
	}

	if ( CharactersInventory->EquippedItem->IsA( AWeapon::StaticClass() ) )
	{
		AWeapon* CurrentlyEquippedWeapon = ( AWeapon* ) CharactersInventory->EquippedItem;
		CurrentlyEquippedWeapon->OnAttack();
	}
}

/*
 *	Controls the aim-offset so that the weapon can aim up and down
 *	TODO: Fix this. The shooting animation call stops the aim offset working.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::AimUp_Down( float InInputVal )
{
	if ( bIsAiming )
	{
		AimValue = InInputVal;
	} else
	{
		AimValue = 0;
	}
}

/*
 *	Finds which enemy is to the right of the enemy you are currently aiming at
 *	then aims at it.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::SwitchToNextEnemy()
{
	if ( GameHasFocus() && bIsAiming && CharactersInventory->EquippedItem )
	{
		// Get all the enemies in the level. [24/7/2016 Matthew Woolley]
		TArray<AActor*> Enemies = GetEnemies();

		// Store the enemy that is closest to the right. [24/7/2016 Matthew Woolley]
		float ClosestToTheRight = -1;
		AActor* ClosestEnemy = nullptr;

		// Get the Character's rotation so we can compare it with the look-at rotation. [24/7/2016 Matthew Woolley]
		float CurrentRotation = GetControlRotation().Yaw;

		// For each enemy. [24/7/2016 Matthew Woolley]
		for ( AActor* Enemy : Enemies )
		{
			// Get the yaw rotation needed to look at the current enemy. [24/7/2016 Matthew Woolley]
			float LookAtRotation = ( ( GetActorLocation() - Enemy->GetActorLocation() ).Rotation().Yaw - 180 );

			// If the rotation is closer than the last enemy AND is to the right. [24/7/2016 Matthew Woolley]
			if ( ( LookAtRotation < ClosestToTheRight || ClosestToTheRight == -1 ) && LookAtRotation >( CurrentRotation ) )
			{
				// Set it as the closest enemy. [24/7/2016 Matthew Woolley]
				ClosestToTheRight = LookAtRotation;
				ClosestEnemy = Enemy;
			}
		}

		// If there is an enemy. [24/7/2016 Matthew Woolley]
		if ( ClosestEnemy )
		{
			// Make Ned look at the found enemy. [16/7/2016 Matthew Woolley]
			FRotator LookAtRotation = ( GetActorLocation() - ClosestEnemy->GetActorLocation() ).Rotation();
			GetController()->SetControlRotation( FRotator( 0, LookAtRotation.Yaw - 180, 0 ) );
		} else
		{
			// Redo the enemy check, however, allow the code to find an enemy that is also to the left. [24/7/2016 Matthew Woolley]
			for ( AActor* Enemy : Enemies )
			{
				float LookAtRotation = ( ( GetActorLocation() - Enemy->GetActorLocation() ).Rotation().Yaw - 180 );

				// If the rotation is closer to 0 than the last enemy. [24/7/2016 Matthew Woolley]
				if ( ( LookAtRotation < ClosestToTheRight || ClosestToTheRight == -1 ) )
				{
					// Set it as the closest enemy. [24/7/2016 Matthew Woolley]
					ClosestToTheRight = LookAtRotation;
					ClosestEnemy = Enemy;
				}
			}
		}

		// If there is an enemy. [24/7/2016 Matthew Woolley]
		if ( ClosestEnemy )
		{
			// Make Ned look at the found enemy. [16/7/2016 Matthew Woolley]
			FRotator LookAtRotation = ( GetActorLocation() - ClosestEnemy->GetActorLocation() ).Rotation();
			GetController()->SetControlRotation( FRotator( 0, LookAtRotation.Yaw - 180, 0 ) );
		}
	}
}

/*
 *	Determines whether the player wishes to reload with a full clip or with a half-full clip.
 *	11/4/2017 - Matthew Woolley
 */
void ARoadFeverCharacterNed::Reload( float InInputVal )
{
	// Only allow reloading if the weapon is being aimed. [8/8/2016 Matthew Woolley]
	if ( !bIsAiming )
		return;

	// The amount of time the button has been pressed down. [26/7/2016 Matthew Woolley]
	static float TimeHeld = 0;
	static bool bCanReload = true;

	// If the user is pressing the reload button. [26/7/2016 Matthew Woolley]
	if ( InInputVal != 0 && bCanReload )
	{
		TimeHeld += GetWorld()->GetDeltaSeconds();
	}

	// If the player has only held the key long enough to preform a full reload. [26/7/2016 Matthew Woolley]
	if ( InInputVal == 0 && TimeHeld > .01f && bCanReload )
	{
		// Check if the player has a weapon to reload. [26/7/2016 Matthew Woolley]
		if ( ( AWeapon* ) CharactersInventory->EquippedItem )
		{
			// Request a full-reload. [26/7/2016 Matthew Woolley]
			AWeapon* EquippedWeapon = ( AWeapon* ) CharactersInventory->EquippedItem;
			EquippedWeapon->Reload( true );
		}

		// Reset the timer. [26/7/2016 Matthew Woolley]
		TimeHeld = 0;
	} else if ( InInputVal == 0 && !bCanReload ) // If the player preformed a secondary reload, and has stopped pressing the key, allow another reload. [26/7/2016 Matthew Woolley]
	{
		bCanReload = true;
	}

	// If the button has been held long enough for the partial reload. [26/7/2016 Matthew Woolley]
	if ( InInputVal != 0 && TimeHeld >= 2 && bCanReload )
	{
		// Check if the player has a weapon to reload. [26/7/2016 Matthew Woolley]
		if ( ( AWeapon* ) CharactersInventory->EquippedItem )
		{
			// Request a full-reload. [26/7/2016 Matthew Woolley]
			AWeapon* EquippedWeapon = ( AWeapon* ) CharactersInventory->EquippedItem;
			EquippedWeapon->Reload( false );
			bCanReload = false;
		}

		// Reset the timer. [26/7/2016 Matthew Woolley]
		TimeHeld = 0;
	}
}

/*
 *	Gets all the enemies within the level that aren't being blocked by anything
 *	such as a wall.
 *	11/4/2017 - Matthew Woolley
 */
TArray<class AActor*> ARoadFeverCharacterNed::GetEnemies()
{
	// The enemies that can be aimed at. [17/7/2016 Matthew Woolley]
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), ARoadFeverEnemy::StaticClass(), Enemies );
	TArray<AActor*> NonBlockedEnemies;

	// Get all of the enemies in the level. [17/7/2016 Matthew Woolley]
	for ( AActor* iActorIterator : Enemies )
	{
		// Get the World object. [16/7/2016 Matthew Woolley]
		UWorld* const World = GetWorld();

		// Collision parameters for the line trace. [16/7/2016 Matthew Woolley]
		FHitResult Hit;
		FCollisionQueryParams Line( FName( "Enemy Line Trace" ), true );
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add( this );
		Line.AddIgnoredActors( IgnoredActors );

		// Make sure there is nothing in the way of the enemy (I.E. a wall). [16/7/2016 Matthew Woolley]
		bool bHasBlockingHit = World->LineTraceSingleByChannel( Hit, this->GetActorLocation(), iActorIterator->GetActorLocation(), WEAPON_TRACE, Line );

		// If there is nothing in the way of the enemy. [16/7/2016 Matthew Woolley]
		if ( bHasBlockingHit && Hit.GetActor() == iActorIterator )
		{
			NonBlockedEnemies.Add( iActorIterator );
		}
	}

	// Return the enemies found. [24/7/2016 Matthew Woolley]
	return NonBlockedEnemies;
}
