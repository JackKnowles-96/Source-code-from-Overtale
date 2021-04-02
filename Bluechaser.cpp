// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Bluechaser.h"
#include "PCstate.h"
#include "Playercharacter.h"
#include "PCBullet.h"


/**
* This class handles the AI for the bonus emeny who grants more score
*/

/// Sets default values 
ABluechaser::ABluechaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chasermeshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bluechasermesh"));
	RootComponent = chasermeshcomp;
	chasermeshcomp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Chashermesh(TEXT("StaticMesh'/Game/Overtale/Models/scorechaser.scorechaser'"));
	if (Chashermesh.Succeeded()) {
		chasermeshcomp->SetStaticMesh(Chashermesh.Object);
	}

	chasermeshcomp->OnComponentHit.AddDynamic(this, &ABluechaser::Hitobject);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		Deathaudio(TEXT("SoundWave'/Game/Overtale/Music/small_blast.small_blast'"));
	if (Deathaudio.Succeeded())
	{
		Deathsound = Deathaudio.Object;
	}

}

/// Called when the game starts or when spawned
void ABluechaser::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	State = Cast<APCstate>(Player->PlayerState);

	Targetlocation = FVector(230.0f, 0.f, 0.f);

	r = FVector::Dist(this->GetActorLocation(), Targetlocation);

	theta = FMath::Acos((this->GetActorLocation().X - Targetlocation.X) / r);

	health = 1;

	this->Randomrot();

	currentstate = 1;
}

/// Called every frame
void ABluechaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	currentstate = ABluechaser::AIlogic();
	   
	AIact(DeltaTime);
}
///uses triggernometry to move around a point
void ABluechaser::Circlemove(float DeltaTime)
{
	Templocation.X = Targetlocation.X + r * FMath::Cos(theta);
	Templocation.Y = Targetlocation.Y + r * FMath::Sin(theta);
	Templocation.Z = this->GetActorLocation().Z;
	
	if (playercirc == 1)
	{
		theta += 0.5f* DeltaTime;
	}
	else
	{
		theta -= 0.5f* DeltaTime;
	}

	this->SetActorLocation(Templocation, false);

	FVector Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(Targetlocation - Currentloc).Rotator();

	this->SetActorRotation(Rot);
}

int ABluechaser::AIlogic()
{
	if (currentstate != 0)
	{
		if (health <= 0) {
			//DEAD STATE
			return 2;
		}
		else
		{
			//Alive
			return 1;
		}
	}
	//waiting
	return 0;
}
///adds a bit of randomnes to the AI movement
void ABluechaser::Randomrot()
{
	playercirc = FMath::RandRange(1, 2);

	UWorld* const World = GetWorld();

	World->GetTimerManager().SetTimer(Changerot, this, &ABluechaser::Randomrot, 12.f);
}
///Hitfunction
void ABluechaser::Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit)
{
	//if PC bullet hits AI loose health
	if (OtherAct->GetClass()->IsChildOf(APCBullet::StaticClass()))
	{
		health = 0;
		OtherAct->Destroy();
		State->setscore(250);
		State->Sethealth(-100);
	}
	else
	{
		chasermeshcomp->MoveIgnoreActors.Add(OtherAct);
	}
}
///Changes the AI state and acts on it
void ABluechaser::AIact(float DeltaTime)
{
	//array holding states
	enum AIstate
	{
		WAIT = 0, ALIVE = 1, DEAD = 2
	};
	//state switch
	switch (currentstate)
	{
	case DEAD:
		ABluechaser::Deadmode();
		break;
	case ALIVE:
		ABluechaser::Circlemove(DeltaTime);
		break;
	case WAIT:
		currentstate = 0;
		break;
	default:
		//default state to wait
		currentstate = 0;
		break;
	}
}
///called when the AI has 0hp
void ABluechaser::Deadmode()
{
	FVector deadlocation;

	deadlocation.X = this->GetActorLocation().X;
	deadlocation.Y = this->GetActorLocation().Y;
	deadlocation.Z = 10000;

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(Respawntimer, this, &ABluechaser::RespawnAI, 40.f);
	//gives the player 50 score
	State->setscore(50);
	State->Sethealth(-10);
	this->SetActorLocation(deadlocation);
	currentstate = 0;
}
///Respawn AI
void ABluechaser::RespawnAI()
{
	FVector respawnloc;

	respawnloc.X = this->GetActorLocation().X;
	respawnloc.Y = this->GetActorLocation().Y;
	respawnloc.Z = 112.000732;

	this->SetActorLocation(respawnloc);
	currentstate = 1;
	health = 1;
}