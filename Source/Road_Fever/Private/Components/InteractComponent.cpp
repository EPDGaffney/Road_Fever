// Copyright Square Crank Games. All Rights Reserved.

#include "Road_Fever.h"
#include "Public/Components/InteractComponent.h"


// Called when the component enters memory [8/6/2015 Matthew Woolley]
UInteractComponent::UInteractComponent()
{
	bAutoActivate = true;
	bAutoRegister = true;
}

// Called when the component leaves memory [8/6/2015 Matthew Woolley]
UInteractComponent::~UInteractComponent()
{

}

// When the character wants to interact with this object [8/6/2015 Matthew Woolley]
void UInteractComponent::OnInteract_Implementation()
{
	// No default behavior for interacting [8/6/2015 Matthew Woolley]
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("No interaction event setup!"));
	GetOwner()->Destroy();
}
