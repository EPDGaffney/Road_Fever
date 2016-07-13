// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Items/Item.h"
#include "Public/Characters/RoadFeverCharacterNed.h"

void AItem::AttachItemToNed_Hand()
{
	ARoadFeverCharacterNed* PlayerCharacter = Cast<ARoadFeverCharacterNed>( GetWorld()->GetFirstPlayerController()->GetPawn() );
	USkeletalMeshComponent* NedsCharacterMesh = PlayerCharacter->GetMesh();

	if ( NedsCharacterMesh )
	{
		AttachRootComponentTo( NedsCharacterMesh, FName( TEXT( "ItemPoint" ) ), EAttachLocation::SnapToTarget );
	}
}
