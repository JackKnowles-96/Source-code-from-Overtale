// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "PCstate.h"



APCstate::APCstate()
{

	Playerhealth = 100;

	Playerscore = 0;

}

void APCstate::Sethealth(int health)
{
	Playerhealth = Playerhealth - health;
	if (Playerhealth <= 0) 
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GAMEOVER");
	}
}

int APCstate::GetPlayerhealth()
{
	return Playerhealth;
}

void APCstate::setscore(int score)
{
	Playerscore = Playerscore + score;
}

int APCstate::GetPlayerscore()
{
	return Playerscore;
}