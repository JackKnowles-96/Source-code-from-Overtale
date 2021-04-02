// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "AITower.h"
#include "PCstate.h"
#include "Playercharacter.h"
#include "Towerbullet.h"
#include "PCBullet.h"
/**
* ... Tower AI ...
*/
/// Sets default values
AAITower::AAITower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//sets the actors mesh
	Towermesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Towermesh"));
	RootComponent = Towermesh;
	Towermesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Towermeshcomp(TEXT("StaticMesh'/Game/Overtale/Models/Itower.Itower'"));
	if (Towermeshcomp.Succeeded()) {
		Towermesh->SetStaticMesh(Towermeshcomp.Object);
	}

	Towermesh->OnComponentHit.AddDynamic(this, &AAITower::Hitobject);
}

/// Called when the game starts or when spawned
void AAITower::BeginPlay()
{
	Super::BeginPlay();
	//Player pawn
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//Player state
	State = Cast<APCstate>(Player->PlayerState);
	//allowed to fire
	abletofire = true;
	//sets the starting health
	health = 10;
	//sets the starting state
	currentstate = 3;
}

/// Called every frame
void AAITower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	playerdis = this->distance(this->GetActorLocation(), Player->GetActorLocation());

	currentstate = AAITower::AIlogic();

	AAITower::AIact(DeltaTime);

}

///collision function
void AAITower::Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit)
{
	//if PC bullet hits AI loose health
	if (OtherAct->GetClass()->IsChildOf(APCBullet::StaticClass()))
	{
		health = health - 10;
		OtherAct->Destroy();
		State->setscore(10);
	}
}

///Distance function
float AAITower::distance(FVector Start, FVector target)
{
	float dis;

	dis = FMath::Sqrt(FMath::Pow((Start.X - target.X), 2) +
		FMath::Pow((Start.Y - target.Y), 2) + FMath::Pow((Start.Z - target.Z), 2));

	return dis;
}
///handles switching states
int AAITower::AIlogic()
{
	if (currentstate != 0 && currentstate != 4)
	{
		if (health <= 0) {
			//DEAD STATE
			return 3;
		}
		if (playerdis < 400)
		{
			//NEAR STATE
			return 1;
		}
		if (playerdis > 400)
		{
			//FAR STATE
			return 2;
		}
	}
	return 0;
}
///Acts based on state
void AAITower::AIact(float DeltaTime)
{
	enum AIstate
	{
		WAIT = 0, NEARSTATE = 1, FARSTATE = 2, DEADSTATE = 3
	};

	//state switch
	switch (currentstate)
	{
	case WAIT:
		currentstate = 0;
		break;
	case NEARSTATE:
		AAITower::Nearmode();
		break;
	case FARSTATE:
		AAITower::Farmode(DeltaTime);
		break;
	case DEADSTATE:
		//set state to wait
		AAITower::Deadmode();
		break;
	default:
		//default state to wait
		currentstate = 0;
		break;
	}
}
///AI has died
void AAITower::Deadmode()
{
	FVector deadlocation;

	deadlocation.X = this->GetActorLocation().X;
	deadlocation.Y = this->GetActorLocation().Y;
	deadlocation.Z = 10000;

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(Respawntimer, this, &AAITower::RespawnAI, 10.f);
	//gives the player 50 score
	State->setscore(50);
	State->Sethealth(-10);
	this->SetActorLocation(deadlocation);
	currentstate = 0;
}
///AI repsawn
void AAITower::RespawnAI()
{
	FVector respawnloc;

	respawnloc.X = this->GetActorLocation().X;
	respawnloc.Y = this->GetActorLocation().Y;
	respawnloc.Z = 112.000732;

	this->SetActorLocation(respawnloc);
	currentstate = 2;
	health = 10;
}
///close to player 
void AAITower::Nearmode()
{
	FVector targetloc = Player->GetActorLocation();
	FVector Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(targetloc - Currentloc).Rotator();
	this->SetActorRotation(Rot);
	AAITower::firebullet();
}
///far from player
void AAITower::Farmode(float DeltaTime)
{
	FRotator Rot = this->GetActorRotation();
	Rot.Yaw = Rot.Yaw + (100 * DeltaTime);
	this->SetActorRotation(Rot);
	AAITower::firebullet();
}

///fires the bullet if the AI is able
void AAITower::firebullet()
{
	if (abletofire)
	{
		//sets the rotation to match AI
		FRotator Spawnrot = this->GetActorRotation();
		FVector Spawnloc = GetActorLocation();
		//sets the offset
		FVector ForwVector = GetActorForwardVector();
		ForwVector = ForwVector * 80;
		Spawnloc = Spawnloc + ForwVector;
		//points to world
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			//spawns bullet is world is not null
			ATowerbullet* NewActor = World->SpawnActor<ATowerbullet>(Spawnloc, Spawnrot);
		}
		//sets timer till next shot
		World->GetTimerManager().SetTimer(Timebetweenshots, this, &AAITower::Allowfire, 0.6f);
		abletofire = false;
	}
}
///resets fire ability
void AAITower::Allowfire()
{
	abletofire = true;
}