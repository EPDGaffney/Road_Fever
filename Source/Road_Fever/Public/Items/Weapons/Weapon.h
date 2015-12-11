// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Public/Items/Item.h"
#include "Weapon.generated.h"

// The properties to customize how the weapon behaves [11/20/2015 Matthew Woolley]
USTRUCT( BlueprintType)
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()

public:
	// How close an enemy has to be (in cm) for damage to be applied [11/20/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Attack")
	float Range;

	// The amount of damage the attack applies [11/20/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Attack")
	int32 Damage;

	// The time between each attack [11/20/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Attack")
	float CoolDownTime;

	// Whether the weapons is cooling down or not [11/20/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bIsCoolingDown;
};



/**
 * Base class for all weapons
 */
UCLASS(abstract, Blueprintable, BlueprintType)
class ROAD_FEVER_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	// Called when this Weapon enters memory [11/20/2015 Matthew Woolley]
	AWeapon();

	// Called when this Weapon leaves memory [11/20/2015 Matthew Woolley]
	~AWeapon();

	// Called every frame [11/20/2015 Matthew Woolley]
	virtual void Tick(float DeltaTime) override;

	// Called when the player wishes to attack with this weapon [11/20/2015 Matthew Woolley]
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void OnAttack();

	// The properties for customizing this weapon [11/20/2015 Matthew Woolley]
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Attack")
	FWeaponProperties WeaponProperties;

	
	
};
