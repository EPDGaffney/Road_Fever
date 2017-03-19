// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Items/Item.h"
#include "Inventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROAD_FEVER_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	// Called every frame. [10/8/2016 Matthew Woolley]
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

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

	// Called when to ask the player whether or not they wish to pick the item. [22/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Pickup" )
	void OpenPickupConfirmation();

	// Called when the confirmation screen should be dismissed. [22/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Pickup" )
	void ClosePickupConfirmation();
};
