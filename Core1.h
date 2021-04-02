// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Core1.generated.h"

UCLASS()
class OVERTALEHACKER_API ACore1 : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACore1();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	//Mesh class
	class UStaticMeshComponent* Coremesh;
	//Hit function
	UFUNCTION()
		void Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit);

private:
	//Holds player data
	class APawn* Player;
	//holds player state
	class APCstate* State;
	//AI HEALTH
	int health;
	//AI Current state
	int currentstate;

	int AIlogic();
	
	float playerdis;

	float distance(FVector Start, FVector target);

	FTimerHandle Timebetweenshots;

	FTimerHandle Respawntimer;

	void AIact(float DeltaTime);

	void Rangedmode();

	void firebullet();

	bool abletofire;

	void Allowfire();

	void Deadmode();

	void RespawnAI();

	void Closemode(float DeltaTime);

	void Chasemode(float DeltaTime);

};
