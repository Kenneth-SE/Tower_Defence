// Fill out your copyright notice in the Description page of Project Settings.


#include "RadiusEffectTowerActor.h"

// Sets default values
ARadiusEffectTowerActor::ARadiusEffectTowerActor()
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
	TargetRadius = 800;
	TriggerVolume->SetSphereRadius(TargetRadius);
	MinDamage = 1;
	MaxDamage = 2;
	RateOfFire = 1;
	Price = 300;
}

// The timer for periodically attacking
void ARadiusEffectTowerActor::CreateAttackTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARadiusEffectTowerActor::Attack, RateOfFire, true);
}

// Called when attacking agent
void ARadiusEffectTowerActor::Attack()
{
	// Check all agents
	for (int32 i = 0; i < Agents.Num(); i++)
	{
		// Calculate damage
		const int DAMAGE = FMath::RandRange(MinDamage, MaxDamage);
		Agents[i]->DamageSelf(DAMAGE);
	}

	// Show attack animation
	OnAttack.Broadcast();

	// Remove all destroyed agents
	int32 i = 0;
	while (i < Agents.Num())
	{
		if (ARadiusEffectTowerActor::IsValidAgent(Agents[i]))
		{
			// Increment if agent is alive
			i++;
		}
		else
		{
			// Remove dead agent
			Agents.RemoveAt(i);
		}
	}

	// Clear timer if there is no more agents remaining within radius
	if (Agents.Num() == 0)
	{
		ARadiusEffectTowerActor::ClearAgentAttackTimer();
	}
}

// When an object begins to overlap the trigger box
void ARadiusEffectTowerActor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if overlapped actor is an instance of an agent actor
	// Jamie Dale's comment: https://answers.unrealengine.com/questions/19256/how-to-check-if-an-actor-is-from-a-certain-class.html
	if (OtherActor->IsA(AAgentActor::StaticClass()))
	{
		// Cast to agent actor class and add target to the list
		// kylawl's comment: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/40599-casting-c-syntax-and-ue-syntax
		AAgentActor* Agent = Cast<AAgentActor>(OtherActor);
		if (ARadiusEffectTowerActor::IsValidAgent(Agent))
		{
			Agents.Add(Agent);
			
			// Check if the timer is active and not already make it so
			if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
			{
				ARadiusEffectTowerActor::CreateAttackTimer();
			}
		}
	}
}

// When an object stops overlapping the trigger box
void ARadiusEffectTowerActor::OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if overlapped actor is an instance of an agent actor
	// Jamie Dale's comment: https://answers.unrealengine.com/questions/19256/how-to-check-if-an-actor-is-from-a-certain-class.html
	if (OtherActor->IsA(AAgentActor::StaticClass()))
	{
		// Cast to agent actor class and add target to the list
		// kylawl's comment: https://forums.unrealengine.com/development-discussion/c-gameplay-programming/40599-casting-c-syntax-and-ue-syntax
		AAgentActor* Agent = Cast<AAgentActor>(OtherActor);
		if (ARadiusEffectTowerActor::IsValidAgent(Agent))
		{
			Agents.Remove(Agent);

			// Clear timer if there is no more agents within radius
			if (Agents.Num() == 0)
			{
				ARadiusEffectTowerActor::ClearAgentAttackTimer();
			}
		}
	}
}

// Clears the timer handle
void ARadiusEffectTowerActor::ClearAgentAttackTimer()
{
	// Check if the timer is active and not already make it so
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

// Called when the game starts or when spawned
void ARadiusEffectTowerActor::BeginPlay()
{
	Super::BeginPlay();

	// Set the trigger box radius
	TriggerVolume->SetSphereRadius(TargetRadius);

	// Add functions to trigger
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ARadiusEffectTowerActor::OnSphereOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ARadiusEffectTowerActor::OnSphereOverlapEnd);
}

// Checks if the agent is still alive
bool ARadiusEffectTowerActor::IsValidAgent(AAgentActor* agent)
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
void ARadiusEffectTowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

