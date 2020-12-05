// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgentActor.h"
#include "Components/StaticMeshComponent.h"
#include "SpawnerPlatformActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAgentSpawn);

UCLASS()
class ASSIGNMENT_2_API ASpawnerPlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerPlatformActor();
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	// Waypoints
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		TArray<FVector> FirstPathWayPoints;
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		TArray<FVector> SecondPathWayPoints;
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		TArray<FVector> ThirdPathWayPoints;
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		TArray<FVector> FourthPathWayPoints;

	// Interval
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		float FirstSpawnObjectInterval;
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		float SecondSpawnObjectInterval;
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		float ThirdSpawnObjectInterval;
	UPROPERTY(EditAnywhere, Category = "SpawnObject")
		float FourthSpawnObjectInterval;

	// Object to spawn into the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
		TSubclassOf<AAgentActor> FirstSpawnObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
		TSubclassOf<AAgentActor> SecondSpawnObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
		TSubclassOf<AAgentActor> ThirdSpawnObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
		TSubclassOf<AAgentActor> FourthSpawnObject;

	// Event
	UPROPERTY(BlueprintAssignable)
		FOnAgentSpawn OnAgentSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
		AAgentActor* AgentReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Variable to handle spawning
	float FirstSpawnObjectCountdown;
	float SecondSpawnObjectCountdown;
	float ThirdSpawnObjectCountdown;
	float FourthSpawnObjectCountdown;

	// Choose path to base
	void SpawnAndChoosePath(TSubclassOf<AAgentActor> AgentToSpawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
