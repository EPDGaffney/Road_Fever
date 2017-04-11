// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Structs/InventoryItem.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROAD_FEVER_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	/*
	*	Sets the inventory to default as closed.
	*	Sets up the ticking so that the TickComponent function fires even when paused.
	*	11/4/2017 - Matthew Woolley
	*/
	UInventory();

	/*
	*	Ensures that slots with no ammo or stack size is removed from the inventory.
	*	11/4/2017 - Matthew Woolley
	*/
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	/*
	*	Toggles the state of the inventory, then calls the function that opens
	*	and closes the UI.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	/*
	*	Spawns the UI for the player to interact with.
	*	Sets the input mode to focus onto the UI.
	*	Shows the mouse cursor.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInv();

	/*
	*	Destroys the UI.
	*	Sets the input mode to focus into the game.
	*	Hides the mouse cursor.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInv();

	/*
	*	Shows a confirmation screen so that the user can pickup or cancel an item interaction.
	*	Sets the focus of the game onto the UI.
	*	Shows the mouse cursor
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Pickup" )
	void OpenPickupConfirmation();

	/*
	*	Destroys the UI.
	*	Sets the input mode to focus into the game.
	*	Hides the mouse cursor.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Pickup" )
	void ClosePickupConfirmation();

	// Whether or not the inventory is open. [21/11/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bOpen;

	// The widget that is spawned to show the UI. [30/11/2015 Matthew Woolley]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> InventoryUIWidgetTemplate;

	// The current instance of the widget in game. [30/11/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UUserWidget* InventoryUIWidgetInstance;

	// The slots within the inventory. [21/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Inventory" )
	TArray<FInventoryItem> ItemSlots;

	// The current tool-tip for the most recently clicked slot. [21/12/2015 Matthew Woolley]
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Inventory" )
	FString CurrentToolTip;

	// The slot that is currently being used. [21/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Item" )
	int32 CurrentActiveSlot;

	// The slot that the currently equipped item is owned by. [13/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Equipping" )
	int32 EquippedItemsSlot;

	// The currently equipped item (spawned in-game). [13/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Equipping" )
	AItem* EquippedItem;

	// The widget that is spawned to ask for pickup confirmation. [22/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Pickup" )
	TSubclassOf<UUserWidget> ItemPickupConfirmationTemplate;

	// The current instance of the confirmation screen that is in game. [22/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Pickup" )
	UUserWidget* ItemPickupConfirmationInstance;
};
