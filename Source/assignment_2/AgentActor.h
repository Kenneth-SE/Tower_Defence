// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AgentActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AAgentActor*, AgentReference);

UCLASS()
class ASSIGNMENT_2_API AAgentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAgentActor();

	// Visible component
	UPROPERTY(EditAnywhere, Category = "Agent Properties")
		UStaticMeshComponent* VisibleComponent;

	// Current health of the agent
	UPROPERTY(EditAnywhere, Category = "Agent Properties")
		int32 Health;

	// The original (max) health value of the agent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Properties")
		int32 OriginalHealth;

	// Setup particle system for agent death
	// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Particles/UParticleSystem/index.html
	UPROPERTY(EditAnywhere, Category = "Agent Properties")
		UParticleSystem* DeathParticleEffect;

	// Setup particle system for agent spawn
	// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Particles/UParticleSystem/index.html
	UPROPERTY(EditAnywhere, Category = "Agent Properties")
		UParticleSystem* SpawnParticleEffect;

	// Agent spawn sound
	// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/PlaySoundAtLocation/2/index.html
	UPROPERTY(EditAnywhere, Category = "Agent Properties")
		USoundBase* SpawnSoundEffect;

	// Agent death sound
	// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/PlaySoundAtLocation/2/index.html
	UPROPERTY(EditAnywhere, Category = "Agent Properties")
		USoundBase* DeathSoundEffect;

	// Add pathway points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agent Properties")
		TArray<FVector> Waypoints;

	// Agent Death Event
	UPROPERTY(BlueprintAssignable)
		FOnDeath OnDeath;

	// Declare event input
	// ZioYuri78's comment: https://www.reddit.com/r/unrealengine/comments/5cwogc/declare_multicast_delegate_oneparam_function/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent Properties")
		AAgentActor* AgentReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Waypoint tolerance
	float Tolerance;

	// Speed the actor travels
	float Speed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Keep track of current waypoint
	int32 WaypointIndex;

	// Damaged dealt to the agent
	void DamageSelf(int32 Damage);

	// Damaged dealt to the agent
	void DamageBase();
};
