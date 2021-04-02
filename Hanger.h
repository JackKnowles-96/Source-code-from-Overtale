// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Hanger.generated.h"

UCLASS()
class OVERTALEHACKER_API AHanger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHanger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	class UStaticMeshComponent* hangermeshcomp;

private:
	void spawning();

	bool abletospawn;

	float offset;

	FTimerHandle spawner;
	
	float spawnrate;

	void Allowspawn();

	class APawn* Player;
};
