// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class UUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROAD_FEVER_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	// Called when the player toggles the inventory [11/21/2015 Matthew Woolley]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	// Called when the player opens the inventory [11/20/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInv();

	// Called when the player closes the inventory [11/20/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void CloseInv();

	// Called when the player wants to use an item in the inventory [11/20/2015 Andreas Gustavsen]
	void OnUse();

	// Called when the player wants to examine an item in the inventory [11/20/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnExamine();

	// Called when the player wants to combine an item in the inventory [11/20/2015 Andreas Gustavsen]
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnCombine();

	// Whether or not the inventory is open [11/21/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bIsOpen;

	// The widget that is spawned to show the UI [11/30/2015 Matthew Woolley]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> InventoryUIWidgetTemplate;

	// [11/30/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UUserWidget* InventoryUIWidgetInstance;
};
