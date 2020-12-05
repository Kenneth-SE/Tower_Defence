// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalPlatformActor.h"

// Sets default values
AGoalPlatformActor::AGoalPlatformActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up visible component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	VisibleComponent->SetupAttachment(RootComponent);
	TriggerVolume->SetupAttachment(RootComponent);

	TriggerVolume->InitBoxExtent(FVector(100, 100, 100));
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AGoalPlatformActor::OnOverlapBegin);

	Health = 4;
}

// When an object begins to overlap the trigger box
void AGoalPlatformActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Dynamically casts to see if the actor is an agent and if so destroy it
	AAgentActor* AgentActor = dynamic_cast<AAgentActor*>(OtherActor);
	if (AgentActor)
	{
		AgentActor->DamageBase();
		this->Health -= 1;
		OnAgentDestroyed.Broadcast();
	}
}

// Called when the game starts or when spawned
void AGoalPlatformActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGoalPlatformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

