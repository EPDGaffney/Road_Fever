// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "RoadFeverCharacterNed.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Dummy Classes/RoadFeverCameraDummy.h"
#include "Public/Inventory/Inventory.h"
#include "Public/Items/Weapons/Weapon.h"
#include "Public/AI/Enemy/RoadFeverEnemy.h"



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

	QuickTurnSpeed = 500;
	TurnSensitivity = 2;

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

// Called at the beginning of game-play. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::BeginPlay()
{
	// Create the ARoadFeverCameraDummy class for the camera-system. [11/12/2015 Matthew Woolley]
	CameraDummy = Cast<ARoadFeverCameraDummy>( GetWorld()->SpawnActor( ARoadFeverCameraDummy::StaticClass() ) );
	CameraDummy->AttachRootComponentToActor( this, NAME_None, EAttachLocation::SnapToTarget );
}

// Called every frame. [10/12/2015 Matthew Woolley]
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
		InInputComponent->BindAxis( "Aim", this, &ARoadFeverCharacterNed::AimUp_Down );
		InInputComponent->BindAction( "Attack", IE_Pressed, this, &ARoadFeverCharacterNed::OnAttack );

		InInputComponent->BindAction( "ToggleInventory", IE_Pressed, CharactersInventory, &UInventory::ToggleInventory );

		InInputComponent->BindAction( "SwitchEnemy", IE_Pressed, this, &ARoadFeverCharacterNed::SwitchToNextEnemy );

		InInputComponent->BindAxis( "Reload", this, &ARoadFeverCharacterNed::Reload );
	}
}

// Returns the text that the inventory screen should display for the health value. [25/12/2015 Matthew Woolley]
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

// Returns the text that the inventory screen should display for the blood loss value. [25/12/2015 Matthew Woolley]
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
				if ( ( ItemToAdd.bIsClip && ItemToAdd.CurrentAmmo == CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo ) || !ItemToAdd.bIsClip )
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
			CharactersInventory->ItemSlots[ iSlotIterator ].bIsEquipable = ItemToAdd.bIsEquipable;
			CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = ItemToAdd.CurrentAmmo;

			// If this item is a weapon. [24/7/2016 Matthew Woolley]
			if ( ItemToAdd.bIsWeapon )
			{
				// Setup the weapon stats. [24/7/2016 Matthew Woolley]
				CharactersInventory->ItemSlots[ iSlotIterator ].MaxAmmo = ItemToAdd.MaxAmmo;
				CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = ItemToAdd.CurrentAmmo;
				CharactersInventory->ItemSlots[ iSlotIterator ].AmmoType = ItemToAdd.AmmoType;
			} else
			{
				// Setup the weapon stats. [24/7/2016 Matthew Woolley]
				CharactersInventory->ItemSlots[ iSlotIterator ].MaxAmmo = 0;
				CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = 0;
				CharactersInventory->ItemSlots[ iSlotIterator ].AmmoType = NULL;
			}

			// If this is a clip ammo type (other ammo types are added normally). [25/7/2016 Matthew Woolley]
			if ( ItemToAdd.bIsClip )
			{
				CharactersInventory->ItemSlots[ iSlotIterator ].bIsClip = ItemToAdd.bIsClip;
				CharactersInventory->ItemSlots[ iSlotIterator ].CurrentAmmo = ItemToAdd.CurrentAmmo;
			}

			// Tell the code that this item was added successfully. [5/3/2016 Matthew Woolley]
			return true;
		}
	}

	// If the item wasn't added, tell the code that it failed. [5/3/2016 Matthew Woolley]
	return false;
}

// Returns true if the game has focus (no menu is open, no dialog etc.). [5/4/2016 Matthew Woolley]
bool ARoadFeverCharacterNed::GameHasFocus()
{
	return ( !CharactersInventory->bIsOpen && !CharactersInventory->ItemPickupConfirmationInstance );
}

// Moves the Character in the X axis. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::MoveForward( float InInputVal )
{
	if ( !GameHasFocus() || bIsAiming )
	{
		_move( 0, EAxis::X );
	} else
	{
		_move( InInputVal, EAxis::X );
	}
}

// Called when the player wishes to turn. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::Turn( float InInputVal )
{
	if ( !GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None )
	{
		AddControllerYawInput( ( GameHasFocus() ? InInputVal : 0 ) );
	}
}

// Called when the player attempts to interact. [10/12/2015 Matthew Woolley]
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
			if ( iActorIterator->IsA( AItem::StaticClass() ) && ( ( AItem* ) iActorIterator )->bCanBePickedUp )
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
				IgnoredActors.Add( CameraDummy );
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

// Called when the player wishes to sprint. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnBeginSprint()
{
	if ( !GameHasFocus() || bIsAiming )
	{
		return;
	}

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
	if ( !GameHasFocus() || bIsAiming )
	{
		return;
	}

	if ( DegreesToTurn <= 0 && MoveForwardAxis < 0 )
	{
		DegreesToTurn = 180;
	}
}

// Called when the player begins aiming. [10/12/2015 Matthew Woolley]
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

// Called when the player stops aiming. [10/12/2015 Matthew Woolley]
void ARoadFeverCharacterNed::OnEndAim()
{
	bIsAiming = false;
}

// Calls the attack function on the currently equipped weapon. [14/7/2016 Matthew Woolley]
void ARoadFeverCharacterNed::OnAttack()
{
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

// Controls the character's up and down aiming. [8/7/2016 Matthew Woolley]
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

// Auto-aims at the enemy that is the right of the currently-aimed at enemy. [24/7/2016 Matthew Woolley]
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
		} else // If we didn't find an enemy. [24/7/2016 Matthew Woolley]
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

// Called when the player wishes to reload. [26/7/2016 Matthew Woolley]
void ARoadFeverCharacterNed::Reload( float InInputVal )
{
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
		GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, TEXT( "Resquested Full Reload" ) );

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
		GEngine->AddOnScreenDebugMessage( -1, 1.f, FColor::Red, TEXT( "Requested Partial Reload" ) );

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

void ARoadFeverCharacterNed::_move( float InInputVal, EAxis::Type InMoveAxis )
{
	// Update the Axis Input so the animations can respond. [8/7/2016 Matthew Woolley]
	MoveForwardAxis = InInputVal;

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

// Gets all the enemies that aren't blocked by walls, etc.. [24/7/2016 Matthew Woolley]
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
		IgnoredActors.Add( CameraDummy );
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
