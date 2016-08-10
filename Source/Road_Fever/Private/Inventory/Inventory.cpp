// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Inventory/Inventory.h"
#include "Public/Items/Item.h"
#include "Public/UMG.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Characters/RoadFeverCharacterNed.h"



// Sets default values for this component's properties
UInventory::UInventory()
{
	bIsOpen = false;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame. [10/8/2016 Matthew Woolley]
void UInventory::TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction )
{
	for ( int32 iItemSlotIterator = 0; iItemSlotIterator < ItemSlots.Num() ; iItemSlotIterator++)
	{
		FInventoryItem SlotItem = ItemSlots[ iItemSlotIterator ];

		// True if there are items in this slot. [10/8/2016 Matthew Woolley]
		bool bHasItems = ( SlotItem.CurrentItemStack > 0 );

		// True if there is ammo in this/these clip(s). [10/8/2016 Matthew Woolley]
		bool bHasAmmo =  SlotItem.bIsClip ? ( ( SlotItem.CurrentAmmo > 0 ) && ( !SlotItem.bIsWeapon ) ) : bHasItems;

		// If there are no items, or no ammo. [10/8/2016 Matthew Woolley]
		if ( !bHasItems || !bHasAmmo )
		{
			// Empty the slot. [10/8/2016 Matthew Woolley]
			ItemSlots[ iItemSlotIterator ].bIsClip = false;
			ItemSlots[ iItemSlotIterator ].bIsEquipable = false;
			ItemSlots[ iItemSlotIterator ].bIsEquipped = false;
			ItemSlots[ iItemSlotIterator ].bIsWeapon = false;
			ItemSlots[ iItemSlotIterator ].CurrentAmmo = 0;
			ItemSlots[ iItemSlotIterator ].CurrentItemStack = 0;
			ItemSlots[ iItemSlotIterator ].DisplayIcon = nullptr;
			ItemSlots[ iItemSlotIterator ].DisplayName = "";
			ItemSlots[ iItemSlotIterator ].ItemClass = NULL;
			ItemSlots[ iItemSlotIterator ].ItemToolTip = "";
			ItemSlots[ iItemSlotIterator ].MaxAmmo = 0;
			ItemSlots[ iItemSlotIterator ].MaxItemStack = 0;
		}
	}
}

// Toggles the inventory. [22/11/2015 Matthew Woolley]
void UInventory::ToggleInventory()
{
	ARoadFeverCharacterNed* PlayerCharacter = ( ARoadFeverCharacterNed* ) GetWorld()->GetFirstPlayerController()->GetPawn();

	// If the game should allow input. [29/7/2016 Matthew Woolley]
	if ( !PlayerCharacter->bIsAiming && ( PlayerCharacter->GameHasFocus() || bIsOpen ) )
	{
		bIsOpen = !bIsOpen;
		bIsOpen ? OpenInv() : CloseInv();
	}
}

// Open inventory [20/11/2015 Andreas Gustavsen]
void UInventory::OpenInv()
{
	// Spawn the InventoryUIWidget if a valid template was provided. [30/11/2015 Matthew Woolley]
	if ( InventoryUIWidgetTemplate )
	{
		UWorld* const World = GetWorld();
		InventoryUIWidgetInstance = CreateWidget<UUserWidget>( World, InventoryUIWidgetTemplate );
	}

	// If the widget was created without any errors. [30/11/2015 Matthew Woolley]
	if ( InventoryUIWidgetInstance && !InventoryUIWidgetInstance->GetIsVisible() )
	{
		// Add it to the view port. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance->AddToViewport();

		// Pause the game. [29/7/2016 Matthew Woolley]
		APlayerController* PlayerController = ( APlayerController* ) GEngine->GetFirstLocalPlayerController( GetWorld() );
		PlayerController->SetPause( true );

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus( InventoryUIWidgetInstance->GetCachedWidget() );
		PlayerController->SetInputMode( Mode );
		PlayerController->bShowMouseCursor = true;
	}
}

// Close inventory [20/11/2015 Andreas Gustavsen]
void UInventory::CloseInv()
{
	// If there is a widget. [30/11/2015 Matthew Woolley]
	if ( InventoryUIWidgetInstance && InventoryUIWidgetInstance->GetIsVisible() )
	{
		// Remove it from sight. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance->RemoveFromParent();

		// Pause the game. [29/7/2016 Matthew Woolley]
		APlayerController* PLayerController = ( APlayerController* ) GEngine->GetFirstLocalPlayerController( GetWorld() );
		PLayerController->SetPause( false );

		FInputModeGameOnly Mode;
		PLayerController->SetInputMode( Mode );
		FSlateApplication::Get().SetFocusToGameViewport();
		PLayerController->bShowMouseCursor = false;

		// Delete the pointer. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance = nullptr;
	}
}

// Called when to ask the player whether or not they wish to pick the item. [22/7/2016 Matthew Woolley]
void UInventory::OpenPickupConfirmation()
{
	// Spawn the InventoryUIWidget if a valid template was provided. [22/7/2016 Matthew Woolley]
	if ( ItemPickupConfirmationTemplate )
	{
		UWorld* const World = GetWorld();
		ItemPickupConfirmationInstance = CreateWidget<UUserWidget>( World, ItemPickupConfirmationTemplate );
	}

	// If the widget was created without any errors. [22/7/2016 Matthew Woolley]
	if ( ItemPickupConfirmationInstance && !ItemPickupConfirmationInstance->GetIsVisible() )
	{
		// add it to the view port. [22/7/2016 Matthew Woolley]
		ItemPickupConfirmationInstance->AddToViewport();

		// Allow the widget to accept input. [22/7/2016 Matthew Woolley]
		APlayerController* const Controller = GetWorld()->GetFirstPlayerController();

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus( ItemPickupConfirmationInstance->GetCachedWidget() );
		Controller->SetInputMode( Mode );
		Controller->bShowMouseCursor = true;
	}
}

// Called when the confirmation screen should be dismissed. [22/7/2016 Matthew Woolley]
void UInventory::ClosePickupConfirmation()
{
	// If there is a widget. [22/7/2016 Matthew Woolley]
	if ( ItemPickupConfirmationInstance && ItemPickupConfirmationInstance->GetIsVisible() )
	{
		// Remove it from sight. [22/7/2016 Matthew Woolley]
		ItemPickupConfirmationInstance->RemoveFromParent();

		// Make the input return to the game. [22/7/2016 Matthew Woolley]
		APlayerController* const Controller = GetWorld()->GetFirstPlayerController();

		FInputModeGameOnly Mode;
		Controller->SetInputMode( Mode );
		FSlateApplication::Get().SetFocusToGameViewport();
		Controller->bShowMouseCursor = false;

		// Delete the pointer. [22/7/2016 Matthew Woolley]
		ItemPickupConfirmationInstance = nullptr;
	}
}
