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

	// Called when the player wants to use an item in the inventory [20/11/2015 Andreas Gustavsen]
	void OnUse();

	// Called when the player wants to examine an item in the inventory [20/11/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnExamine();

	// Called when the player wants to combine an item in the inventory [11/20/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnCombine();

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
};
