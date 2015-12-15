// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROAD_FEVER_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called when this Component enters memory. [15/12/2015 Matthew Woolley]
	UInteractionComponent();

	// Called when the player wishes to interact with this object. [15/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Interaction" )
	void OnInteract();
	virtual void OnInteract_Implementation();
		
	
};
