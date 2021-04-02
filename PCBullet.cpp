// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "PCBullet.h"
#include "Chaser.h"

// Sets default values
APCBullet::APCBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		bulletmesh(TEXT("StaticMesh'/Game/Overtale/Models/PCbullet.PCbullet'"));
	Pbulletmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));

	if (bulletmesh.Succeeded()) {
		Pbulletmesh->SetStaticMesh(bulletmesh.Object);
	}


	Pbulletmesh->SetupAttachment(RootComponent);
	Pbulletmesh->SetCollisionProfileName("PCbullet");
	RootComponent = Pbulletmesh;

	Pbulletmovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PCbulletmovement1"));
	Pbulletmovement->UpdatedComponent = Pbulletmesh;
	Pbulletmovement->InitialSpeed = 1000.f;
	Pbulletmovement->MaxSpeed = 1000.f;
	Pbulletmovement->bRotationFollowsVelocity = true;
	Pbulletmovement->bShouldBounce = false;
	Pbulletmovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 1.5f;

	FVector scale(1.0f, 1.0f, 1.0f);

	this->SetActorScale3D(scale);
}

// Called when the game starts or when spawned
void APCBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APCBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}