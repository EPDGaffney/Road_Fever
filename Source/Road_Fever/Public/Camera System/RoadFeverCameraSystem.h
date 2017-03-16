// Copyright Square Crank Games. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "RoadFeverCameraSystem.generated.h"

// Handles the position of the camera in-game. [11/12/2015 Matthew Woolley]
USTRUCT( BlueprintType )
struct FCameraPositioning
{
	GENERATED_USTRUCT_BODY()

public:
	// The location for this camera. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	FVector Location;

	// The rotation of this camera. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	FRotator Rotation;
};

UCLASS()
class ROAD_FEVER_API ARoadFeverCameraSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called when this Actor enters memory. [11/12/2015 Matthew Woolley]
	ARoadFeverCameraSystem();

	// Called at the beginning of game-play. [11/12/2015 Matthew Woolley]
	virtual void BeginPlay() override;

	// Called when something enters this camera's trigger area. [11/12/2015 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Detection" )
	void OnActorEnter( class UPrimitiveComponent* InPrimitiveComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherPrimitiveComponent, int32 InInt, bool InSweepResult, const FHitResult& InFHit );

	// Called when something leaves this camera's trigger area. [20/1/2016 Matthew Woolley]
	UFUNCTION( BlueprintCallable, Category = "Detection" )
	void OnActorLeave( class UPrimitiveComponent* InPrimitiveCompnent, AActor* InOtherActor, UPrimitiveComponent* InOtherPrimitiveComponent, int32 InInt );

	// The area that, when entered, triggers the Character's camera location. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Detection" )
	class UBoxComponent* TriggerArea;

	// The transform that, when triggered, the Character's camera moves to. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	FCameraPositioning CameraPosition;

	// The camera that gets shown in the editor to help the LDs position them. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere )
	class UCameraComponent* EditorCameraReference;

	// Whether or not the game starts using this camera. [11/12/2015 Matthew Woolley]
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Rendering" )
	bool bIsPrimaryCamera;

};
