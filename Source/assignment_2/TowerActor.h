// Fill out your copyright notice in the Description page of Project Settings.

/*NOT USED*/

#pragma once

#include "AgentActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "TowerActor.generated.h"

UCLASS()
class ASSIGNMENT_2_API ATowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerActor();

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		UStaticMeshComponent* VisibleComponent;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		float TargetRadius;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		int32 MinDamage;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		int32 MaxDamage;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		float RateOfFire;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		AAgentActor* target;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		TArray<AAgentActor*> Agents;

	UPROPERTY(EditAnywhere, Category = "Tower properties")
		UCapsuleComponent* TriggerVolume;

	UFUNCTION(BlueprintCallable)
		void Attack();

	UFUNCTION(BlueprintCallable)
		void CreateAttackTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timer handler
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
