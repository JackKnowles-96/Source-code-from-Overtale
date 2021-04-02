// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Playercharacter.generated.h"

UCLASS()
class OVERTALEHACKER_API APlayercharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayercharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void MoveForwards(float Val);

	void MoveRight(float Val);

	void firebutton(float Val);

	void Facemouse();

private:
	class UStaticMeshComponent* Playermeshcomp;

	float Currentforwardspeed;

	float Currentrightspeed;

	float currentfire;

	bool abletofire;

	float firespeed;

	float bulletoffset;

	FTimerHandle Timebetweenshots;

	class USoundBase* bulletsound;

	void Allowfire();

	void firing();

	class USoundBase* lasersound;

	class USoundBase* Deathsound;

};
