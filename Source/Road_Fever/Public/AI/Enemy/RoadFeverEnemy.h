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
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	int32 EnemyHealth;

	// Called when this enemy no longer has any health. [15/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Actions" )
	void Die();

	// Takes away health from this enemy. [15/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void TakeDamage( int32 InDamage );

	// The speed this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	float MoveSpeed;

protected:
	// The slowest this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	float MinMoveSpeed;

	// The fastest this enemy can move. [23/1/2017 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Movement" )
	float MaxMoveSpeed;


};
