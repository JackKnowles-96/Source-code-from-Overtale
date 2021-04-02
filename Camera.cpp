// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Camera.h"


// Sets default values
ACamera::ACamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	RootComponent = CameraComponent;
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();

	FVector MyFVector(0.0f, 0.0f, 1600.0f);
	CameraComponent->SetWorldLocation(MyFVector);

	FRotator CamRotator(-90.0f, 0.0f, -90.0f);
	CameraComponent->SetWorldRotation(CamRotator);

	UWorld* const World = GetWorld();
	if (World != NULL) {
		APlayerController* const PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			PlayerController->SetViewTarget(this);
		}
	}

}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

