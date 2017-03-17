// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "RoadFeverEnemy.generated.h"

UCLASS()
class ROAD_FEVER_API ARoadFeverEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Setup default values for this enemy. [15/7/2016 Matthew Woolley]
	ARoadFeverEnemy();

	void BeginPlay() override;

	// The health value of this enemy. [15/7/2016 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Stats" )
	int32 EnemyHealth;

	// Called when this enemy no longer has any health. [15/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Actions" )
	void Die();
	void Die_Implementation();

	// Takes away health from this enemy. [15/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Attack" )
	void TakeDamage( int32 InDamage );
	void TakeDamage_Implementation( int32 InDamage );

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
	// Whether the player is in the inventory, or item confirmation screen. [17/3/2017 Matthew Woolley]
	UFUNCTION( BlueprintPure, Category = "UI" )
	const bool PlayerIsPaused();

	// The slowest this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement" )
	float MinMoveSpeed;

	// The fastest this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement" )
	float MaxMoveSpeed;


};
