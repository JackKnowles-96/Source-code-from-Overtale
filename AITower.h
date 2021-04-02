// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AITower.generated.h"

UCLASS()
class OVERTALEHACKER_API AAITower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAITower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit);

private:
	class UStaticMeshComponent* Towermesh;
	
	//Holds player data
	class APawn* Player;
	//holds player state
	class APCstate* State;

	float health;

	bool abletofire;

	float playerdis;

	float distance(FVector Start, FVector target);

	int currentstate;

	int AIlogic();

	void AIact(float DeltaTime);

	void Deadmode();
	
	void Nearmode();

	void Farmode(float DeltaTime);

	void RespawnAI();

	void firebullet();

	void Allowfire();

	FTimerHandle Respawntimer;

	FTimerHandle Timebetweenshots;

};
