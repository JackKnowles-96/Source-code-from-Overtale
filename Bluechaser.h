// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bluechaser.generated.h"

UCLASS()
class OVERTALEHACKER_API ABluechaser : public AActor
{
	GENERATED_BODY()
		//PI
		const float M_PI = 3.14159265358979323846;
	
public:	
	// Sets default values for this actor's properties
	ABluechaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit);

private:

	///vectors

	FVector Templocation;

	FVector Targetlocation;

	///floats

	float r;

	float theta;

	float health;


	///classes

	class UStaticMeshComponent* chasermeshcomp;

	class USoundBase* Deathsound;

	class APawn* Player;

	class APCstate* State;

	///ints

	int currentstate;

	int playercirc;

	///functions

	int AIlogic();

	void Circlemove(float DeltaTime);

	void Randomrot();

	void AIact(float DeltaTime);

	void Deadmode();

	void RespawnAI();

	///Timers
	FTimerHandle Changerot;

	FTimerHandle Respawntimer;
};
