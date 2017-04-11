// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "RoadFeverEnemy.generated.h"

UCLASS()
class ROAD_FEVER_API ARoadFeverEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	/*
	*	Gives the enemy its default health and speed.
	*	Sets up the location and rotation for the mesh so that it doesn't need to be fiddled with at run-time when the mesh is assigned.
	*	11/4/2017 - Matthew Woolley
	*/
	ARoadFeverEnemy();

	/*
	*	Get a random speed for the enemy to move at, so that they don't all look like they are walking in-step.
	*	Get a random mesh from a pool of meshes so that not all enemies look the same, even when they are the same type.
	*	Call the Super:: so that any BP implemented version of this works.
	*/
	void BeginPlay() override;

	/*
	*	Called when this enemy has taken damage and has no remaining health.
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Actions" )
	void Die();
	void Die_Implementation();

	/*
	*	Call BP functionality (Spawns effects).
	*	Take the amount of damage dealt away from the enemy's health.
	*	Figure out whether the enemy has died or not.
	*	Return the amout of health remaining of the enemy.
	*	11/4/2017 - Matthew Woolley
	*/
	virtual float TakeDamage( float InDamage, struct FDamageEvent const& InDamageEvent, class AController* InEventInstigator, class AActor* InDamageCauser ) override;
	

	// The health value of this enemy. [15/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Stats" )
	int32 EnemyHealth;

	// The speed this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement" )
	float MoveSpeed;

	// The meshes that this enemy type can use. [3/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Config" )
	TArray<USkeletalMesh*> EnemyMeshes;

	// The item that is dropped so that the player can loot this enemy. [8/2/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	TSubclassOf<class AItem> ItemsInInventory;

protected:
	/*
	*	Returns whether or not the player is playing (false) or in a menu (true).
	*	11/4/2017 - Matthew Woolley
	*/
	UFUNCTION( BlueprintPure, Category = "UI" )
	const bool IsPlayerPaused();

	// The slowest this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement" )
	float MinMoveSpeed;

	// The fastest this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement" )
	float MaxMoveSpeed;


};
