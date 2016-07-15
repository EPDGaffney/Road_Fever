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

	// The health value of this enemy. [15/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	int32 EnemyHealth;

	// Called when this enemy no longer has any health. [15/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Actions" )
	void Die();

	// Takes away health from this enemy. [15/7/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Attack" )
	void TakeDamage( int32 InDamage );

	// A visual representation of what this enemy's health is [FOR DEBUGGING]. [15/7/2016 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Stats" )
	UTextRenderComponent* HealthVisual;

};
