// Fill out your copyright notice in the Description page of Project Settings.

/*NOT USED*/

#include "TowerActor.h"

// Sets default values
ATowerActor::ATowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up visible component
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	Scene->SetupAttachment(RootComponent);
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(Scene);
	TriggerVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Volume"));
	TriggerVolume->SetupAttachment(Scene);

	// Initialise variables
	TargetRadius = 800;
	MinDamage = 1;
	MaxDamage = 1;
	RateOfFire = 1;
}

void ATowerActor::CreateAttackTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATowerActor::Attack, RateOfFire, true);
} 


void ATowerActor::Attack()
{
	if (target)
	{
		// Calculate damage
		int damage = FMath::RandRange(MinDamage, MaxDamage);
		target->DamageSelf(damage);
	}

	UE_LOG(LogTemp, Warning, TEXT("ATTACKING.\n"));
}

// Called when the game starts or when spawned
void ATowerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check all agents
	if (Agents.Num() > 0)
	{
		for (int i = 0; i < Agents.Num(); i++)
		{
			if (Agents[i])
			{
				FVector CurrentLocation = GetActorLocation();
				FVector TargetLocation = Agents[i]->GetActorLocation();

				if (FVector::Dist(CurrentLocation, TargetLocation) < TargetRadius)
				{
					// Calculate damage
					int damage = FMath::RandRange(MinDamage, MaxDamage);
					Agents[i]->DamageSelf(damage);
				}
			}
		}
	}
}

