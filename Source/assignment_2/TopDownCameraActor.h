// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "TopDownCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_2_API ATopDownCameraActor : public ACameraActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
