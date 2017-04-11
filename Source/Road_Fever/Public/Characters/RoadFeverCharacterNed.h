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
	/*
	*	Sets up the collection area for collecting items.
	*	Sets up the shooting point for weapons' ray tracing.
	*	Sets up the default settings like sensitivity.
	*	Creates the camera that is used by the CameraSystem.
	*	Creates the inventory for later use.
	*	Sets up default stats like Health and BloodLoss.
	*	Turns ticking on.
	*	11/4/2017 - Matthew Woolley
	*/
	ARoadFeverCharacterNed();

	/*
	*	Switches animations based on whether anything is equipped or whether the player is aiming.
	*	Turns the character if the DegreesToTurn is not 0 or less (used for quick turning).
	*	11/4/2017 - Matthew Woolley
	*/
	virtual void Tick( float InDeltaSeconds ) override;

	/*
	*	Sets up the default input bindings.
	*	11/4/2017 - Matthew Woolley
	*/
	virtual void SetupPlayerInputComponent( class UInputComponent* InInputComponent ) override;

	/*
	*	Returns a string that is used by the UI to display the current state of the player's health.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Stats" )
	FString UpdateHealthMessage();

	/*
	*	Returns a string that is used by the UI to display the current state of the player's blood loss.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Stats" )
	FString UpdateBloodMessage();

	/*
	*	Finds a stack that the item being added can be added into (true).
	*	Finds an empty slot that the item being added can be added into (true).
	*	Leaves the item if there was no space for it (false).
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Item" )
	bool AddItemToInventory( struct FInventoryItem ItemToAdd );

	/*
	*	Whether or not this player is in a menu, paused (true), or playing (false).
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintPure, Category = "Misc" )
	const bool GameHasFocus();

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

	// The current level of the blood loss for the player. [25/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	int32 BloodLoss;

	// The camera that is currently active. [20/1/2016 Matthew Woolley]
	class ARoadFeverCameraSystem* CurrentCamera;

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

	// Whether debug traces and what-not are shown. [11/4/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Rendering" )
	bool bIsDebugging;

protected:
	/*
	*	If the player isn't aiming, it assigns the MoveForwardAxis variable to be equal to that of the player input
	*	the AnimationBP then uses this for RootMotion and moves the character Forward (+) and backward (-).
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void MoveForward( float InInputVal );
	
	/*
	*	If the player is allowed to move, turn the character right (+) or left (-) with the sensitivity
	*	that has been set.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void Turn( float InInputVal );


	/*
	*	If the game has focus, and the player is not aiming, interact with the nearest item.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Interact" )
	void OnCharacterInteract();
	virtual void OnCharacterInteract_Implementation();

	/*
	*	If the game isn't paused and the player isn't aiming, set Sprinting to true
	*	so that the AnimationBP can change the animation to a sprinting one.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void OnBeginSprint();

	/*
	*	Set Sprinting to false so that the AnimationBP uses default walking animations.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void OnEndSprint();

	/*
	*	Sets the DegreesToTurn to 180° so that the player turns around (done in Tick).
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Movement" )
	void OnBeginQuickTurn();

	/*
	*	When the player begins aiming, find the nearest enemy and look at it.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void OnBeginAim();

	/*
	*	Used so that the animation can go back to not-aiming and so that the player can move again.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void OnEndAim();

	/*
	*	Fires a ray-trace from the weapon which then determines whether and enemy was hit.
	*	Logic for shooting is done within the Weapon class.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void OnAttack();

	/*
	*	Controls the aim-offset so that the weapon can aim up and down
	*	TODO: Fix this. The shooting animation call stops the aim offset working.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void AimUp_Down( float InInputVal );

	/*
	*	Finds which enemy is to the right of the enemy you are currently aiming at
	*	then aims at it.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void SwitchToNextEnemy();

	/*
	*	Determines whether the player wishes to reload with a full clip or with a half-full clip.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void Reload( float InInputVal );

	// Used to detect items when interacting. [10/12/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Interact" )
	class UBoxComponent* CollectionArea;

private:
	/*
	*	Gets all the enemies within the level that aren't being blocked by anything
	*	such as a wall.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( Category = "Attack" )
	TArray<class AActor*> GetEnemies();

	// Used to turn the character 180 degrees. [18/7/2016 Matthew Woolley]
	UPROPERTY()
	float DegreesToTurn;

};
