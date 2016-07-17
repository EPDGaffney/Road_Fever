// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Public/Items/Item.h"
#include "Weapon.generated.h"

// The properties to customize how the weapon behaves [20/11/2015 Matthew Woolley]
USTRUCT( BlueprintType )
struct FWeaponProperties
{
	GENERATED_USTRUCT_BODY()

public:
	// The damage this weapon does between the weapon and the effective firing range. [17/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	int32 MaxDamage;

	// The damage this weapon does between the weapon and the maximum range. [17/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	int32 MinDamage;

	// The range that does full damage (In CM). [17/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	float EffectiveRange;

	// The range of which this weapon will still deal damage (In CM). [17/7/2016 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	float MaximumRange;

	// The time between each attack [20/11/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	float CoolDownTime;

	// Whether the weapons is cooling down or not [20/11/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, Category = "Attack" )
	bool bIsCoolingDown;
};



/**
 * Base class for all weapons
 */
UCLASS( abstract, Blueprintable, BlueprintType )
class ROAD_FEVER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Called when this Weapon enters memory [20/11/2015 Matthew Woolley]
	AWeapon();

	// Called when this Weapon leaves memory [20/11/2015 Matthew Woolley]
	~AWeapon();

	// Called every frame [20/11/2015 Matthew Woolley]
	virtual void Tick( float DeltaTime ) override;

	// Called when the player wishes to attack with this weapon [20/11/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Attack" )
	void OnAttack();
	virtual void OnAttack_Implementation();

	// The properties for customizing this weapon [20/11/2015 Matthew Woolley]
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly, Category = "Attack" )
	FWeaponProperties WeaponProperties;
};
