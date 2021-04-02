// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "OvertaleHackerGameMode.h"

#include "Playercharacter.h"
#include "PCstate.h"
#include "GameHUD.h"

AOvertaleHackerGameMode::AOvertaleHackerGameMode()
{
	DefaultPawnClass = APlayercharacter::StaticClass();

	PlayerStateClass = APCstate::StaticClass();

	HUDClass = AGameHUD::StaticClass();

}

