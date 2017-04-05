// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Items/Item.h"
#include "RoadFeverCharacterNed.generated.h"

UCLASS()
class ROAD_FEVER_API ARoadFeverCharacterNed : public ACharacter
{
	GENERATED_BODY()

public:
	// Called when this Character enters memory. [10/12/2015 Matthew Woolley]
	ARoadFeverCharacterNed();

	// Called every frame. [10/12/2015 Matthew Woolley]
	virtual void Tick( float InDeltaSeconds ) override;

	// Called to bind player input. [10/12/2015 Matthew Woolley]
	virtual void SetupPlayerInputComponent( class UInputComponent* InInputComponent ) override;

	// The current input from the player for forwards-movement. [10/12/2015 Matthew Woolley]
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Movement" )
	float MoveForwardAxis;

	// Whether or not the player is aiming. [10/12/2015 Matthew Woolley]
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Attack" )
	bool bIsAiming;

	// The speed that the Character quick-turns at. [10/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	float QuickTurnSpeed;

	// The sensitivity of the turning input. [10/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	float TurnSensitivity;

	// Whether or not the Character is sprinting. [10/12/2015 Matthew Woolley]
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Movement" )
	bool bIsSprinting;

	// The camera that will render the scene. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	class UCameraComponent* CharactersCamera;

	// The inventory that the player has. [12/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	class UInventory* CharactersInventory;

	// The current level of health that the player has. [25/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	int32 Health;

	// Returns the text that the inventory screen should display for the health value. [25/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Stats" )
	FString UpdateHealthMessage();

	// The current level of the blood loss for the player. [25/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	int32 BloodLoss;

	// Returns the text that the inventory screen should display for the blood loss value. [25/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Stats" )
	FString UpdateBloodMessage();

	// The camera that is currently active. [20/1/2016 Matthew Woolley]
	class ARoadFeverCameraSystem* CurrentCamera;

	// Called when an item is to be added to the inventory. [5/3/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Item" )
	bool AddItemToInventory( struct FInventoryItem ItemToAdd );

	// Returns true if the game has focus (no menu is open, no dialog etc.). [5/4/2016 Matthew Woolley]
	UFUNCTION( BlueprintPure, Category = "Misc" )
	const bool GameHasFocus();

	// The value of which the character is looking up or down. [8/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	float AimValue;

	// The Animation Blueprint to use when when no item is equipped (or when an item doesn't have its own). [18/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Animation" )
	TSubclassOf<UAnimInstance> DefaultAnimation;

	// The item that is currently being interacted with. [22/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Interaction" )
	class AItem* ItemBeingInteractedWith;

	// The transform and rotation that each weapon uses for attacking. [1/3/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Attack" )
	UArrowComponent* ShootFromPoint;

protected:
	// Moves the Character in the X axis. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void MoveForward( float InInputVal );
	
	// Called when the player wishes to turn. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void Turn( float InInputVal );

	// Used to detect items when interacting. [10/12/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Interact" )
	class UBoxComponent* CollectionArea;

	// Called when the player attempts to interact. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Interact" )
	void OnCharacterInteract();
	virtual void OnCharacterInteract_Implementation();

	// Called when the player wishes to sprint. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void OnBeginSprint();

	// Called when the player stops sprinting. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void OnEndSprint();

	// Called when the player wishes to do a quick-turn. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void OnBeginQuickTurn();

	// Called when the player begins aiming. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void OnBeginAim();

	// Called when the player stops aiming. [10/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void OnEndAim();

	// Calls the attack function on the currently equipped weapon. [14/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void OnAttack();

	// Controls the character's up and down aiming. [8/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void AimUp_Down( float InInputVal );

	// Auto-aims at the enemy that is the right of the currently-aimed at enemy. [24/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void SwitchToNextEnemy();

	// Called when the player wishes to reload. [26/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void Reload( float InInputVal );

private:
	// Used to turn the character 180 degrees. [18/7/2016 Matthew Woolley]
	UPROPERTY()
	float DegreesToTurn;

	// Gets all the enemies that aren't blocked by walls, etc.. [24/7/2016 Matthew Woolley]
	UFUNCTION( Category = "Attack" )
	TArray<class AActor*> GetEnemies();
};
