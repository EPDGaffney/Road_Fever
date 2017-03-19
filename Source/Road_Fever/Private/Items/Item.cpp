// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Items/Item.h"
#include "Public/Characters/RoadFeverCharacterNed.h"
#include "Components/ArrowComponent.h"
#include "Public/Items/Ammo/Ammo.h"
#include "Public/Items/Weapons/Weapon.h"



// Called when this Item is created. [14/7/2016 Matthew Woolley]
AItem::AItem()
{
	// Create Item's root component. [14/7/2016 Matthew Woolley]
	DirectionArrow = CreateDefaultSubobject<UArrowComponent>( TEXT( "Direction Arrow" ) );
	RootComponent = DirectionArrow;

	// Create the default detection box. [14/7/2016 Matthew Woolley]
	DetectionBox = CreateDefaultSubobject<UBoxComponent>( TEXT( "Detection Box" ) );
	DetectionBox->SetBoxExtent( FVector( 32, 32, 32 ) );
	DetectionBox->RelativeLocation = FVector( 0, 0, 32 );
	DetectionBox->SetupAttachment( RootComponent );
	DetectionBox->SetCollisionProfileName( "ItemPickup" );

	// Setup this Item's mesh. [14/7/2016 Matthew Woolley]
	ItemsMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Item's Mesh" ) );
	ItemsMesh->SetupAttachment( RootComponent );
	ItemsMesh->SetCollisionProfileName( "NoCollision" );

	// Setup some generic properties. [20/7/2016 Matthew Woolley]
	bPickupable = true;
}

void AItem::AttachItemToNed_Hand()
{
	ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );
	USkeletalMeshComponent* NedsCharacterMesh = PlayerCharacter->GetMesh();
	bPickupable = false;

	if ( NedsCharacterMesh )
	{
		AttachRootComponentTo( NedsCharacterMesh, FName( TEXT( "ItemPoint" ) ), EAttachLocation::SnapToTarget );
		DetectionBox->DestroyComponent();
	}
}
