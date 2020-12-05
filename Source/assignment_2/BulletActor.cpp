// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up visible component
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	Scene->SetupAttachment(RootComponent);
	this->VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	this->VisibleComponent->SetupAttachment(Scene);
	this->TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Volume"));
	this->TriggerVolume->SetupAttachment(Scene);

	// Initialise variables
	Accuracy = 75;
	Speed = 4000;
	this->MinDamage = 1;
	this->MaxDamage = 1;
}

// When an object begins to overlap the trigger box
void ABulletActor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if overlapped actor is an instance of an agent actor
	// Jamie Dale's comment: https://answers.unrealengine.com/questions/19256/how-to-check-if-an-actor-is-from-a-certain-class.html
	if (OtherActor->IsA(AAgentActor::StaticClass()))
	{
		// Play hit particle effect when agent hit
		if (HitParticleEffect)
		{
			// Setup hit particle effect
			FTransform Transform = GetActorTransform();

			// Show particle mid height of agent
			// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
			const FVector TRANSLATION = Transform.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));
			Transform = FTransform(Transform.GetRotation(), TRANSLATION, Transform.GetScale3D() * 2);

			// Spawn particle effect
			// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/SpawnEmitterAtLocation/1/index.html
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleEffect, Transform, true, EPSCPoolMethod::None, true);
		}

		// Play hit sound
		if (HitSoundEffect)
		{
			// Setup death particle effect
			const FTransform TRANSFORM = GetActorTransform();

			// Sound particle mid height of agent
			// https://docs.unrealengine.com/en-US/API/Runtime/Core/Math/FTransform/index.html
			const FVector LOCATION = TRANSFORM.GetTranslation() + FVector(0, 0, (VisibleComponent->Bounds.BoxExtent.Z));

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSoundEffect, LOCATION, FRotator::ZeroRotator, HitSoundEffect->GetVolumeMultiplier(), HitSoundEffect->GetPitchMultiplier(), 0.0f);
		}
		
		// Cast to agent actor class and add target to the list
		// kylawl's comment: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/40599-casting-c-syntax-and-ue-syntax
		AAgentActor* Agent = Cast<AAgentActor>(OtherActor);
		Agent->DamageSelf(FMath::RandRange(this->MinDamage, this->MaxDamage));
		
		// Bullet is consumed
		this->Destroy();
	}
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	this->TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnSphereOverlapBegin);

	// Bullet accuracy
	FRotator Rotation = GetActorRotation();

	// Calculate accuracy
	const int32 ACCURACY_PROBABILITY = FMath::RandRange(1, 100);
	if (ACCURACY_PROBABILITY > Accuracy)
	{
		// If greater than 75% then the bullet becomes inaccurate
		Rotation += FRotator(0, 25, 0);
	}
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move in a direction
	SetActorLocation(GetActorLocation() + DeltaTime * Speed * GetActorForwardVector());
}

