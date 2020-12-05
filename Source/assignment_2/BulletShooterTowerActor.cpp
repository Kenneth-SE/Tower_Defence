// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletShooterTowerActor.h"

// Sets default values
ABulletShooterTowerActor::ABulletShooterTowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up visible component
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	Scene->SetupAttachment(RootComponent);
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(Scene);
	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Volume"));
	TriggerVolume->SetupAttachment(Scene);

	// Initialise variables
	TargetRadius = 1200;
	MinDamage = 1;
	MaxDamage = 3;
	TriggerVolume->SetSphereRadius(TargetRadius);
	RateOfFire = 1;
	Price = 200;
}

// Creates the timer for periodically attacking
void ABulletShooterTowerActor::CreateAttackTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABulletShooterTowerActor::Attack, RateOfFire, true);
}

// Called when attacking agent
void ABulletShooterTowerActor::Attack()
{
	// Get first agent
	AAgentActor* Target = nullptr;
	if (Agents.Num() > 0)
	{
		Target = Agents[0];
	}
	
	// Check first agent
	if (ABulletShooterTowerActor::IsValidAgent(Target))
	{
		// Show attack animation
		OnAttack.Broadcast();
	}

	// Check if target is destroyed
	if (!ABulletShooterTowerActor::IsValidAgent(Target))
	{
		// Remove dead agent
		Agents.Remove(Target);
	}

	// Clear timer if there is no more agents remaining within radius
	if (Agents.Num() == 0)
	{
		ABulletShooterTowerActor::ClearAgentAttackTimer();
	}
}

// When an object begins to overlap the trigger box
void ABulletShooterTowerActor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if overlapped actor is an instance of an agent actor
	// Jamie Dale's comment: https://answers.unrealengine.com/questions/19256/how-to-check-if-an-actor-is-from-a-certain-class.html
	if (OtherActor->IsA(AAgentActor::StaticClass()))
	{
		// Cast to agent actor class and add target to the list
		// kylawl's comment: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/40599-casting-c-syntax-and-ue-syntax
		AAgentActor* Agent = Cast<AAgentActor>(OtherActor);
		if (ABulletShooterTowerActor::IsValidAgent(Agent))
		{
			Agents.Add(Agent);

			// Check if the timer is active and not already make it so
			if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
			{
				ABulletShooterTowerActor::CreateAttackTimer();
			}
		}
	}
}

// When an object stops overlapping the trigger box
void ABulletShooterTowerActor::OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if overlapped actor is an instance of an agent actor
	// Jamie Dale's comment: https://answers.unrealengine.com/questions/19256/how-to-check-if-an-actor-is-from-a-certain-class.html
	if (OtherActor->IsA(AAgentActor::StaticClass()))
	{
		// Cast to agent actor class and add target to the list
		// kylawl's comment: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/40599-casting-c-syntax-and-ue-syntax
		AAgentActor* Agent = Cast<AAgentActor>(OtherActor);
		if (ABulletShooterTowerActor::IsValidAgent(Agent))
		{
			Agents.Remove(Agent);

			// Clear timer if there is no more agents within radius
			if (Agents.Num() == 0)
			{
				ABulletShooterTowerActor::ClearAgentAttackTimer();
			}
		}
	}
}

// Clears the timer handler
void ABulletShooterTowerActor::ClearAgentAttackTimer()
{
	// Check if the timer is active and not already make it so
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

// Called when the game starts or when spawned
void ABulletShooterTowerActor::BeginPlay()
{
	Super::BeginPlay();

	// Set the trigger box radius
	TriggerVolume->SetSphereRadius(TargetRadius);

	// Add functions to trigger
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ABulletShooterTowerActor::OnSphereOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ABulletShooterTowerActor::OnSphereOverlapEnd);
}

// Clears the timer handler
bool ABulletShooterTowerActor::IsValidAgent(AAgentActor* agent)
{
	// Check if agent exists
	if (!agent)
	{
		return false;
	}

	// Check if agent is in the process of being destroyed
	// Rama's comment: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/11774-pointer-still-pointing-to-something-after-calling-destroy
	if (!agent->IsValidLowLevel())
	{
		return false;
	}
	if (agent->IsPendingKill())
	{
		return false;
	}

	return true;
}

// Called every frame
void ABulletShooterTowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get first agent
	AAgentActor* Target = nullptr;
	if (Agents.Num() > 0)
	{
		Target = Agents[0];
	}

	// Face towards the target agent
	if (IsValidAgent(Target))
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Z = 0;
		Direction.Normalize();

		const FVector Forward = FVector(1, 0, 0);

		const float DOT = FVector::DotProduct(Forward, Direction);
		const float DET = Forward.X * Direction.Y + Forward.Y * Direction.X;
		const float RAD = FMath::Atan2(DET, DOT);
		const float DEG = FMath::RadiansToDegrees(RAD);

		SetActorRotation(FRotator(0, DEG, 0));
	}
}