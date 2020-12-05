// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentActor.h"

// Sets default values
AAgentActor::AAgentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up visible component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	// Setup mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> StatueMeshObj(TEXT("/Game/StarterContent/Props/SM_Statue.SM_Statue"));

	if (StatueMeshObj.Succeeded())
	{
		VisibleComponent->SetStaticMesh(StatueMeshObj.Object);
	}

	// Set up scale
	VisibleComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	// Initialise waypoint index
	WaypointIndex = 0;
	Speed = 0;
	Health = 3;
	OriginalHealth = Health;
	Tolerance = 10.0f;
	Waypoints = TArray<FVector>();
}

// Called when the game starts or when spawned
void AAgentActor::BeginPlay()
{
	Super::BeginPlay();

	// Let speed be overriden manually in blueprints
	if (VisibleComponent)
	{
		// Tie speed to the agents scale
		const float SCALE = VisibleComponent->GetComponentScale().Size();
		Speed = 2000 / SCALE;

		// Play spawn particle effect when spawned
		if (SpawnParticleEffect)
		{
			// Setup death particle effect
			FTransform Transform = GetActorTransform();

			// Show particle mid height of agent
			// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
			const FVector TRANSLATION = Transform.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));
			Transform = FTransform(Transform.GetRotation(), TRANSLATION, Transform.GetScale3D() * 4);

			// Spawn particle effect
			// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/SpawnEmitterAtLocation/1/index.html
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticleEffect, Transform, true, EPSCPoolMethod::None, true);
		}

		// Play death sound
		if (SpawnSoundEffect)
		{
			// Setup death particle effect
			const FTransform TRANSFORM = GetActorTransform();

			// Sound particle mid height of agent
			// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
			const FVector LOCATION = TRANSFORM.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSoundEffect, LOCATION, FRotator::ZeroRotator, SpawnSoundEffect->GetVolumeMultiplier(), SpawnSoundEffect->GetPitchMultiplier(), 0.0f);
		}
	}
}

// Called every frame
void AAgentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if checkpoints array can be accessed
	if ((Waypoints.Num() > 0) && (WaypointIndex < Waypoints.Num()))
	{
		const FVector STARTING_POSITION = GetActorLocation();

		/* Rotate to face waypoint direction */

		// Waypoint direction
		FVector DirectionVector = Waypoints[WaypointIndex] - STARTING_POSITION;	// Direction of waypoint
		DirectionVector.Z = 0;
		DirectionVector.Normalize();

		// Calculate angle between X and Y for Yaw
		const FVector FORWARD_VECTOR = FVector(1, 0, 0);
		const float DOT_PROD = FVector::DotProduct(DirectionVector, FORWARD_VECTOR);
		const float DETERMINANT = DirectionVector.X * FORWARD_VECTOR.Y + DirectionVector.Y * FORWARD_VECTOR.X;
		const float RAD = FMath::Atan2(DETERMINANT, DOT_PROD);
		const float DEG = FMath::RadiansToDegrees(RAD);

		// Rotate Yaw to face waypoint
		const FRotator ROTATOR(0, DEG, 0);
		const FQuat QUAT(ROTATOR);
		SetActorRotation(QUAT);

		/* Local space movement */
		
		// Move towards waypoint when rotation is done (Parallel vectors: Direction and Forward)
		FVector AgentDirectionVector = Waypoints[WaypointIndex] - STARTING_POSITION;
		AgentDirectionVector.Normalize();	// where agent is facing

		// Increment position
		FVector CurrentPosition = STARTING_POSITION + AgentDirectionVector * Speed * DeltaTime;

		// Check if waypoint reached
		if (FVector::Dist(GetActorLocation(), Waypoints[WaypointIndex]) < Tolerance)
		{
			CurrentPosition = Waypoints[WaypointIndex];
			WaypointIndex++;
		}
		SetActorLocation(CurrentPosition);
	}
}

// Damaged dealt to the agent
void AAgentActor::DamageSelf(int32 Damage)
{
	this->Health -= Damage;

	// Change material here to showcase damage
	// Reuben Ward: https://www.youtube.com/watch?v=V5bugE5K-ys
	const float HEALTH_PERCENTAGE = Health / OriginalHealth;
	UMaterialInstanceDynamic* NewMaterial = VisibleComponent->CreateDynamicMaterialInstance(0);
	if (NewMaterial)
	{
		NewMaterial->SetScalarParameterValue(FName("DamageAmount"), HEALTH_PERCENTAGE);
	}
		
	// Check to see if the agent has died
	if (this->Health <= 0)
	{
		// Play explosion particle effect when destroyed
		if (DeathParticleEffect)
		{
			// Setup death particle effect
			FTransform Transform = GetActorTransform();

			// Show particle mid height of agent
			// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
			const FVector TRANSLATION = Transform.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));
			Transform = FTransform(Transform.GetRotation(), TRANSLATION, Transform.GetScale3D() * 2);

			// Spawn particle effect
			// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/SpawnEmitterAtLocation/1/index.html
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticleEffect, Transform, true, EPSCPoolMethod::None, true);
		}
		
		// Play death sound
		if (DeathSoundEffect)
		{
			// Setup death particle effect
			const FTransform TRANSFORM = GetActorTransform();

			// Sound particle mid height of agent
			// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
			const FVector LOCATION = TRANSFORM.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSoundEffect, LOCATION, FRotator::ZeroRotator, DeathSoundEffect->GetVolumeMultiplier(), DeathSoundEffect->GetPitchMultiplier(), 0.0f);
		}

		// Broadcast agent death
		AgentReference = this;
		OnDeath.Broadcast(AgentReference);
		
		// Destroy actor
		this->Destroy();
	}
}

void AAgentActor::DamageBase()
{
	// Play explosion particle effect when destroyed
	if (DeathParticleEffect)
	{
		// Setup death particle effect
		FTransform Transform = GetActorTransform();

		// Show particle mid height of agent
		// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
		const FVector TRANSLATION = Transform.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));
		Transform = FTransform(Transform.GetRotation(), TRANSLATION, Transform.GetScale3D() * 2);

		// Spawn particle effect
		// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/SpawnEmitterAtLocation/1/index.html
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticleEffect, Transform, true, EPSCPoolMethod::None, true);
	}

	// Play death sound
	if (DeathSoundEffect)
	{
		// Setup death particle effect
		const FTransform TRANSFORM = GetActorTransform();

		// Sound particle mid height of agent
		// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
		const FVector LOCATION = TRANSFORM.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSoundEffect, LOCATION, FRotator::ZeroRotator, DeathSoundEffect->GetVolumeMultiplier(), DeathSoundEffect->GetPitchMultiplier(), 0.0f);
	}

	// Destroy actor
	this->Destroy();
}