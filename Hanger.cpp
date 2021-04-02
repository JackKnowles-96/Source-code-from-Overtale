// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Hanger.h"
#include "Chaser.h"


// Sets default values
AHanger::AHanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hangermeshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("chasermesh"));
	RootComponent = hangermeshcomp;
	hangermeshcomp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		hangermesh(TEXT("StaticMesh'/Game/Overtale/Models/Hanger.Hanger'"));
	if (hangermesh.Succeeded()) {
		hangermeshcomp->SetStaticMesh(hangermesh.Object);
	}

}

// Called when the game starts or when spawned
void AHanger::BeginPlay()
{
	Super::BeginPlay();

	abletospawn = true;
	spawnrate = 10.f;
	offset = 75.f;

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

}

// Called every frame
void AHanger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	FVector targetloc = Player->GetActorLocation();
	FVector Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(targetloc - Currentloc).Rotator();
	this->SetActorRotation(Rot);
	

	spawning();
}

void AHanger::spawning() 
{
	if (abletospawn)
	{
		FRotator Spawnrot = this->GetActorRotation();

		FVector Spawnloc = GetActorLocation();

		FVector ForwVector = GetActorForwardVector();

		ForwVector = ForwVector * offset;

		Spawnloc = Spawnloc + ForwVector;

		UWorld* const World = GetWorld();

		if (World != NULL)
		{
			AChaser* NewActor = World->SpawnActor<AChaser>(Spawnloc, Spawnrot);
		}
		World->GetTimerManager().SetTimer(spawner, this, &AHanger::Allowspawn, spawnrate);
		abletospawn = false;
	}
}

void AHanger::Allowspawn()
{
	abletospawn = true;
}

