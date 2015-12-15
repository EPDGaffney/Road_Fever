// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "InteractionComponent.h"


// Called when this Component enters memory. [15/12/2015 Matthew Woolley]
UInteractionComponent::UInteractionComponent()
{

}

// Called when the player wishes to interact with this object. [15/12/2015 Matthew Woolley]
void UInteractionComponent::OnInteract_Implementation()
{
	if ( GEngine )
	{
		GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, TEXT( "Failed to find interaction method on Item" ) );
		GetOwner()->Destroy();
	}
}
