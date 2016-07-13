// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Items/Item.h"
#include "Inventory.generated.h"

class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROAD_FEVER_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	// Called when the player toggles the inventory. [21/11/2015 Matthew Woolley]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	// Called when the player opens the inventory [20/11/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInv();

	// Called when the player closes the inventory [20/11/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInv();

	// Whether or not the inventory is open. [21/11/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bIsOpen;

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
};
