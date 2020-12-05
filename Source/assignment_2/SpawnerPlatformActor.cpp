// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerPlatformActor.h"

// Sets default values
ASpawnerPlatformActor::ASpawnerPlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up visible component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	// Agent spawn setup
	FirstSpawnObjectInterval = 2;
	SecondSpawnObjectInterval = 3;
	ThirdSpawnObjectInterval = 3;
	FourthSpawnObjectInterval = 4;
	FirstSpawnObjectCountdown = 0;
	SecondSpawnObjectCountdown = 3;
	ThirdSpawnObjectCountdown = 4;
	FourthSpawnObjectCountdown = 5;

	// First path
	FirstPathWayPoints.Add(FVector(190, -3520, 513));
	FirstPathWayPoints.Add(FVector(190, -1734, 513));
	FirstPathWayPoints.Add(FVector(-454, -1474, 513));
	FirstPathWayPoints.Add(FVector(-454, -929, 513));
	FirstPathWayPoints.Add(FVector(-751, -677, 513));
	FirstPathWayPoints.Add(FVector(-1125, -677, 513));
	FirstPathWayPoints.Add(FVector(-1448, -356, 513));
	FirstPathWayPoints.Add(FVector(-1448, 2030, 513));
	FirstPathWayPoints.Add(FVector(-1448, 2030, 513));
	FirstPathWayPoints.Add(FVector(-1059, 2463, 513));
	FirstPathWayPoints.Add(FVector(-1059, 3383, 513));

	// Second path
	SecondPathWayPoints.Add(FVector(-1260, -3520, 513));
	SecondPathWayPoints.Add(FVector(-1260, -2715, 513));
	SecondPathWayPoints.Add(FVector(-960, -2386, 513));
	SecondPathWayPoints.Add(FVector(-721, -2386, 513));
	SecondPathWayPoints.Add(FVector(-469, -2018, 513));
	SecondPathWayPoints.Add(FVector(-469, -1447, 513));
	SecondPathWayPoints.Add(FVector(161, -1222, 513));
	SecondPathWayPoints.Add(FVector(161, 859, 513));
	SecondPathWayPoints.Add(FVector(433, 1128, 513));
	SecondPathWayPoints.Add(FVector(712, 1128, 513));
	SecondPathWayPoints.Add(FVector(953, 1392, 513));
	SecondPathWayPoints.Add(FVector(953, 2985, 513));

	// Third path
	ThirdPathWayPoints.Add(FVector(999, -3520, 513));
	ThirdPathWayPoints.Add(FVector(999, -542.0, 513));
	ThirdPathWayPoints.Add(FVector(40, -41, 513));
	ThirdPathWayPoints.Add(FVector(-868, 1467, 513));
	ThirdPathWayPoints.Add(FVector(256, 2856, 513));

	// Fourth path
	FourthPathWayPoints.Add(FVector(-2036, -3520, 513));
	FourthPathWayPoints.Add(FVector(-1506, -1366, 513));
	FourthPathWayPoints.Add(FVector(-727, -199, 513));
	FourthPathWayPoints.Add(FVector(-1844, 1067, 513));
	FourthPathWayPoints.Add(FVector(-2119, 2879, 513));
}

// Called when the game starts or when spawned
void ASpawnerPlatformActor::BeginPlay()
{
	Super::BeginPlay();
}

// Spawns an actor and chooses it's path it will take to the base
void ASpawnerPlatformActor::SpawnAndChoosePath(TSubclassOf<AAgentActor> AgentToSpawn)
{
	// Choose randomly a path to use
	// Documentation: https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FMath/RandRange/1/index.html
	const int32 PATH_CHOICE = FMath::RandRange(1, 4);
	TArray<FVector> ChosenPathway;
	switch(PATH_CHOICE)
	{
		case 1:
			ChosenPathway = FirstPathWayPoints;
			break;
		case 2:
			ChosenPathway = SecondPathWayPoints;
			break;
		case 3:
			ChosenPathway = ThirdPathWayPoints;
			break;
		case 4:
			ChosenPathway = FourthPathWayPoints;
			break;
		default:
			ChosenPathway = FirstPathWayPoints;
	}

	// Spawn agent
	if (AgentToSpawn)
	{
		if (ChosenPathway.Num() > 0)
		{
			AgentReference = GetWorld()->SpawnActor<AAgentActor>(AgentToSpawn, ChosenPathway[0], FRotator::ZeroRotator);
			AgentReference->Waypoints = ChosenPathway;
			OnAgentSpawn.Broadcast();
		}
	}
}

// Called every frame
void ASpawnerPlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Decrement counter for first agent
	FirstSpawnObjectCountdown -= DeltaTime;
	if (FirstSpawnObjectCountdown <= 0)
	{
		SpawnAndChoosePath(FirstSpawnObject);
		
		// Reset first spawn countdown
		FirstSpawnObjectCountdown = FirstSpawnObjectInterval;
	}

	// Decrement counter for second agent
	SecondSpawnObjectCountdown -= DeltaTime;
	if (SecondSpawnObjectCountdown <= 0)
	{
		SpawnAndChoosePath(SecondSpawnObject);

		// Reset second spawn countdown
		SecondSpawnObjectCountdown = SecondSpawnObjectInterval;
	}

	// Decrement counter for third agent
	ThirdSpawnObjectCountdown -= DeltaTime;
	if (ThirdSpawnObjectCountdown <= 0)
	{
		SpawnAndChoosePath(ThirdSpawnObject);

		// Reset second spawn countdown
		ThirdSpawnObjectCountdown = ThirdSpawnObjectInterval;
	}

	// Decrement counter for fourth agent
	FourthSpawnObjectCountdown -= DeltaTime;
	if (FourthSpawnObjectCountdown <= 0)
	{
		SpawnAndChoosePath(FourthSpawnObject);

		// Reset second spawn countdown
		FourthSpawnObjectCountdown = FourthSpawnObjectInterval;
	}
}