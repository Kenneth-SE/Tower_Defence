// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCameraActor.h"

// Called when the game starts or when spawned
void ATopDownCameraActor::BeginPlay()
{
	Super::BeginPlay();

	// Top Down Camera always faces downwards
	FRotator faceDownwards = FRotator(-90, 0, 0);
	SetActorRotation(faceDownwards);
}