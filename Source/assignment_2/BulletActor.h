// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AgentActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "BulletActor.generated.h"

UCLASS()
class ASSIGNMENT_2_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

	// Visible component
	UPROPERTY(EditAnywhere, Category = "Bullet properties")
		UStaticMeshComponent* VisibleComponent;

	// Min damage the bullet does
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet properties")
		int32 MinDamage;

	// Max damage the bullet does
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet properties")
		int32 MaxDamage;

	// Bullet trigger volume
	UPROPERTY(EditAnywhere, Category = "Bullet properties")
		USphereComponent* TriggerVolume;

	// Bullet speed
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet properties")
		float Speed;

	// Bullet accuracy
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bullet properties")
		int32 Accuracy;

	// Setup particle system for bullet hit
	// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Particles/UParticleSystem/index.html
	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
		UParticleSystem* HitParticleEffect;

	// Bullet hit sound
	// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/PlaySoundAtLocation/2/index.html
	UPROPERTY(EditAnywhere, Category = "Bullet Properties")
		USoundBase* HitSoundEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap begin function
	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
