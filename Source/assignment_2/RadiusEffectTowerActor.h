// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgentActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "RadiusEffectTowerActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);

UCLASS()
class ASSIGNMENT_2_API ARadiusEffectTowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadiusEffectTowerActor();

	// Visible component
	UPROPERTY(EditAnywhere, Category = "Tower properties")
		UStaticMeshComponent* VisibleComponent;

	// The detection radius for targeting agents
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tower properties")
		float TargetRadius;

	// Min damage the tower does
	UPROPERTY(EditAnywhere, Category = "Tower properties")
		int32 MinDamage;

	// Max damage the tower does
	UPROPERTY(EditAnywhere, Category = "Tower properties")
		int32 MaxDamage;

	// Tower fire rate
	UPROPERTY(EditAnywhere, Category = "Tower properties")
		float RateOfFire;

	// Tower trigger volume
	UPROPERTY(EditAnywhere, Category = "Tower properties")
		USphereComponent* TriggerVolume;

	// Cost of the tower
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower properties")
		int32 Price;

	// Timer handler
	UPROPERTY(BlueprintAssignable)
		FOnAttack OnAttack;
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when attacking agent
	void Attack();

	// The timer for periodically attacking
	void CreateAttackTimer();

	// Generic timer handle
	FTimerHandle TimerHandle;

	// Overlap begin function
	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Overlap end function
	UFUNCTION()
		void OnSphereOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Clears the timer handle
	void ClearAgentAttackTimer();

	// Checks if the agent is still alive
	bool IsValidAgent(AAgentActor* agent);

	// References to all agents within radius
	TArray<AAgentActor*> Agents;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
