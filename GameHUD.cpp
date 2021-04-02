// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "GameHUD.h"

#include "PCstate.h"

#define LOCTEXT_NAMESPACE "SimpleHUD"

AGameHUD::AGameHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	if (PCcurrentstate) 
	{
		FVector2D Screendim = FVector2D(Canvas->SizeX, Canvas->SizeY);
		FText Healthstring = FText::Format(LOCTEXT("TestFormat", "HEALTH: {0} \nSCORE: {1}"), FText::AsNumber(PCcurrentstate->GetPlayerhealth())
			, FText::AsNumber(PCcurrentstate->GetPlayerscore()));
		FVector2D Textcen = FVector2D(((Canvas->SizeX - Canvas->SizeX) + 100), 10);
		FCanvasTextItem Textitem(Textcen, Healthstring, HUDFont, FLinearColor::White);
		Canvas->DrawItem(Textitem);
	}
	else
	{
		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (Player)
		{
			PCcurrentstate = Cast<APCstate>(Player->PlayerState);
		}
	}
}

#undef LOCTEXT_NAMESPACE