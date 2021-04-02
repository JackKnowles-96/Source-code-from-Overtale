// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Towerbullet.h"
#include "PCBullet.h"
#include "Playercharacter.h"
#include "PCstate.h"
#include "Chaser.h"
#include "Core1.h"
#include "Hanger.h"
#include "AITower.h"

// Sets default values
ATowerbullet::ATowerbullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		AIbullet(TEXT("StaticMesh'/Game/Overtale/Models/Orangebullet.Orangebullet'"));
	AIbulletmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIbulletmesh"));

	if (AIbullet.Succeeded()) {
		AIbulletmesh->SetStaticMesh(AIbullet.Object);
	}


	AIbulletmesh->SetupAttachment(RootComponent);
	AIbulletmesh->SetCollisionProfileName("AIbullet");
	RootComponent = AIbulletmesh;

	AIbulletmovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AIbulletmovement1"));
	AIbulletmovement->UpdatedComponent = AIbulletmesh;
	AIbulletmovement->InitialSpeed = 500.f;
	AIbulletmovement->MaxSpeed = 500.f;
	AIbulletmovement->bRotationFollowsVelocity = true;
	AIbulletmovement->bShouldBounce = false;
	AIbulletmovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 2.8f;

	FVector scale(0.5f, 0.5f, 0.5f);

	this->SetActorScale3D(scale);

	AIbulletmesh->OnComponentHit.AddDynamic(this, &ATowerbullet::Hitobject);
}

// Called when the game starts or when spawned
void ATowerbullet::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	State = Cast<APCstate>(Player->PlayerState);
}

// Called every frame
void ATowerbullet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATowerbullet::Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit)
{
	if (OtherAct->GetClass()->IsChildOf(APCBullet::StaticClass()))
	{
		Destroy();
		OtherAct->Destroy();
		State->setscore(5);
	}
	if (OtherAct->GetClass()->IsChildOf(APlayercharacter::StaticClass()))
	{
		State->Sethealth(10);
		Destroy();
	}
	else
	{
		Destroy();
	}
}

