// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Inventory/Inventory.h"
#include "Public/UMG.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Characters/RoadFeverCharacterNed.h"



// Sets default values for this component's properties
UInventory::UInventory()
{
	bIsOpen = false;
}

// Toggles the inventory. [22/11/2015 Matthew Woolley]
void UInventory::ToggleInventory()
{
	bIsOpen = !bIsOpen;

	bIsOpen ? OpenInv() : CloseInv();
}

// Open inventory [20/11/2015 Andreas Gustavsen]
void UInventory::OpenInv()
{
	// Spawn the InventoryUIWidget if a valid template was provided. [30/11/2015 Matthew Woolley]
	if (InventoryUIWidgetTemplate)
	{
		UWorld* const World = GetWorld();
		InventoryUIWidgetInstance = CreateWidget<UUserWidget>(World, InventoryUIWidgetTemplate);
	}

	// If the widget was created without any errors. [30/11/2015 Matthew Woolley]
	if (InventoryUIWidgetInstance && !InventoryUIWidgetInstance->GetIsVisible())
	{
		// add it to the view port. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance->AddToViewport();

		// Allow the widget to accept input. [30/11/2015 Matthew Woolley]
		APlayerController* const Controller = GetWorld()->GetFirstPlayerController();

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(InventoryUIWidgetInstance->GetCachedWidget());
		Controller->SetInputMode(Mode);
		Controller->bShowMouseCursor = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString("Inventory Opened"));
	}
}

// Close inventory [20/11/2015 Andreas Gustavsen]
void UInventory::CloseInv()
{
	// If there is a widget. [30/11/2015 Matthew Woolley]
	if (InventoryUIWidgetInstance && InventoryUIWidgetInstance->GetIsVisible())
	{
		// Remove it from sight. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance->RemoveFromParent();

		// Make the input return to the game. [30/11/2015 Matthew Woolley]
		APlayerController* const Controller = GetWorld()->GetFirstPlayerController();

		FInputModeGameOnly Mode;
		Controller->SetInputMode(Mode);
		FSlateApplication::Get().SetFocusToGameViewport();
		Controller->bShowMouseCursor = false;

		// Delete the pointer. [30/11/2015 Matthew Woolley]
		InventoryUIWidgetInstance = nullptr;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString("Inventory Closed"));
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
