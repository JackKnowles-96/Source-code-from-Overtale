// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERTALEHACKER_API AGameHUD : public AHUD
{
	GENERATED_BODY()

		class APCstate* PCcurrentstate;

public:

	AGameHUD();

	UPROPERTY()
		UFont* HUDFont;

	virtual void DrawHUD() override;
	
	
};
