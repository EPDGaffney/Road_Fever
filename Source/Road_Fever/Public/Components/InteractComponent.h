// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROAD_FEVER_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called when the component enters memory [8/6/2015 Matthew Woolley]
	UInteractComponent();
	// Called when the component leaves memory [8/6/2015 Matthew Woolley]
	~UInteractComponent();

	// When the character wants to interact with this object [8/6/2015 Matthew Woolley]
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void OnInteract();
	virtual void OnInteract_Implementation();
};
