// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "PCstate.generated.h"

/**
 * 
 */
UCLASS()
class OVERTALEHACKER_API APCstate : public APlayerState
{
	GENERATED_BODY()
	
	int Playerhealth;

	int Playerscore;

public:

	APCstate();

	void Sethealth(int health);

	int GetPlayerhealth();

	void setscore(int score);

	int GetPlayerscore();
};
