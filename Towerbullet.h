// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Towerbullet.generated.h"

UCLASS()
class OVERTALEHACKER_API ATowerbullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerbullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit);

	class UStaticMeshComponent* AIbulletmesh;

	UProjectileMovementComponent* AIbulletmovement;

	class APCstate* State;

	class APawn* Player;
};
