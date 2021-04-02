// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Chaser.generated.h"

UCLASS()
class OVERTALEHACKER_API AChaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChaser();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit);
	
private:
	class UStaticMeshComponent* chasermeshcomp;

	FVector Currentloc;
	FVector targetloc;

	class APawn* Player;
	class APCstate* State;

	class USoundBase* Deathsound;

};
