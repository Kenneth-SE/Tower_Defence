// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "AgentActor.h"
#include "assignment_2Character.h"
#include "GoalPlatformActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAgentDestroyed);

UCLASS()
class ASSIGNMENT_2_API AGoalPlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalPlatformActor();

	// Visible component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	// The detection radius for agents
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerVolume;

	// Object to destroy from the world
	UPROPERTY(EditAnywhere, Category = "Base")
		TSubclassOf<AActor> SpawnObject;

	// Player
	UPROPERTY(EditAnywhere, Category = "Base")
		TSubclassOf<ACharacter> Character;

	// Amount of health the base has
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base")
		int32 Health;

	// Delegate Event
	UPROPERTY(BlueprintAssignable)
		FOnAgentDestroyed OnAgentDestroyed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap begin function
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
