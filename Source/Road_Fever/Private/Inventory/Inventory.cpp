// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Inventory.h"
#include "Runtime/UMG/Public/UMG.h"
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

// Use an item in the inventory [20/11/2015 Andreas Gustavsen]
void UInventory::OnUse()
{

}

// Examine an item in the inventory [20/11/2015 Andreas Gustavsen]
void UInventory::OnExamine()
{

}

// Combine an item in the inventory [20/11/2015 Andreas Gustavsen]
void UInventory::OnCombine()
{

}
