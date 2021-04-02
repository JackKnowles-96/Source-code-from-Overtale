// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Core1.h"
#include "PCBullet.h"
#include "PCstate.h"
#include "Playercharacter.h"
#include "Towerbullet.h"
#include "Chaser.h"
#include "Hanger.h"

/**
* This class handles the main AI for the game
*/

/// Sets default values
ACore1::ACore1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//sets the mesh for the AI
	Coremesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Coremesh"));
	RootComponent = Coremesh;
	Coremesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		core1mesh(TEXT("StaticMesh'/Game/Overtale/Models/AICore.AICore'"));
	if (core1mesh.Succeeded()) {
		Coremesh->SetStaticMesh(core1mesh.Object);
	}
	//Add the hit component
	Coremesh->OnComponentHit.AddDynamic(this, &ACore1::Hitobject);

}

/// Called when the game starts or when spawned
void ACore1::BeginPlay()
{
	Super::BeginPlay();
	//Player pawn
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//Player state
	State = Cast<APCstate>(Player->PlayerState);
	//allowed to fire
	abletofire = true;
	//sets the starting health
	health = 40;
}

/// Called every frame
void ACore1::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	playerdis = this->distance(this->GetActorLocation(), Player->GetActorLocation());

	currentstate = ACore1::AIlogic();

	ACore1::AIact(DeltaTime);

}
/// AI Logic function
int ACore1::AIlogic()
{
	int PChealth = State->GetPlayerhealth();
	//if not waiting and respawning
	if (currentstate != 4)
	{
		//if health is less than 0  enter dead state
		if (health <= 0)
		{
			//DEAD STATE
			return 2;
		}
		//chase the player if they are low on HP
		if (PChealth <= 20)
		{
			return 5;
		}
		//if player is close
		if (playerdis < 600)
		{
			//NEAR STATE
			return 1;
		}
		//if player is far
		if (playerdis > 600)
		{
			//RANGED STATE
			return 0;
		}
	}
	//else wait
	else
	{
		return 4;
	}
	//wait
	return 4;
}

///AI states
void ACore1::AIact(float DeltaTime)
{
	//array holding states
	enum AIstate
	{
		RANGED = 0, NEARPC = 1, DEAD = 2, WAIT = 4, CHASE = 5 
	};
	//state switch
	switch (currentstate)
	{
	case DEAD:
		ACore1::Deadmode();
		break;
	case RANGED:
		ACore1::Rangedmode();
		break;
	case NEARPC:
		ACore1::Closemode(DeltaTime);
		break;
	case WAIT:
		//set state to wait
		currentstate = 4;
		break;
	case CHASE:
		ACore1::Chasemode(DeltaTime);
		break;
	default:
		//default state to wait
		currentstate = 4;
		break;
	}


}
///if the player is to far AI will just shoot at player from range
void ACore1::Rangedmode()
{
	FVector targetloc = Player->GetActorLocation();
	FVector Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(targetloc - Currentloc).Rotator();
	this->SetActorRotation(Rot);
	ACore1::firebullet();
}
///This state is used when the player is close and allows the AI to move
void ACore1::Closemode(float DeltaTime)
{
	FVector targetloc = Player->GetActorLocation();
	FVector Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(targetloc - Currentloc).Rotator();
	this->SetActorRotation(Rot);

	const FVector Movementloc = FVector(0.f, 200.f * DeltaTime, 0.f);
	AddActorLocalOffset(Movementloc, true);

	ACore1::firebullet();
}
///Moves the AI off the stage and changes state to respawn
void ACore1::Deadmode()
{
	FVector deadlocation;

	deadlocation.X = this->GetActorLocation().X;
	deadlocation.Y = this->GetActorLocation().Y;
	deadlocation.Z = 10000;

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(Respawntimer, this, &ACore1::RespawnAI, 20.f);
	//gives the player 200 score
	State->setscore(200);
	State->Sethealth(-20);
	this->SetActorLocation(deadlocation);

	currentstate = 4;

}
///moves the AI back to the stage
void ACore1::RespawnAI()
{
	FVector respawnloc;

	respawnloc.X = this->GetActorLocation().X;
	respawnloc.Y = this->GetActorLocation().Y;
	respawnloc.Z = 112.000732;

	this->SetActorLocation(respawnloc);

	currentstate = 0;

	health = 40;
}
///fires the bullet if the AI is able
void ACore1::firebullet()
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
		World->GetTimerManager().SetTimer(Timebetweenshots, this, &ACore1::Allowfire, 0.6f);
		abletofire = false;
	}
}
///Resets the fire ability
void ACore1::Allowfire()
{
	abletofire = true;
}
///AI Hit oject function
void ACore1::Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit)
{
	//if PC bullet hits AI loose health
	if (OtherAct->GetClass()->IsChildOf(APCBullet::StaticClass()))
	{
		health = health - 10;
		OtherAct->Destroy();
		State->setscore(10);
	}
	//If hit chaser add to ignore list
	if (OtherAct->GetClass()->IsChildOf(AChaser::StaticClass()))
	{
		Coremesh->MoveIgnoreActors.Add(OtherAct);
	}
	//If hit Hanger add to ignore list
	if (OtherAct->GetClass()->IsChildOf(AHanger::StaticClass()))
	{
		Coremesh->MoveIgnoreActors.Add(OtherAct);
	}
}
///Distance function
float ACore1::distance(FVector Start, FVector target)
{
	float dis;

	dis = FMath::Sqrt(FMath::Pow((Start.X - target.X), 2) +
		FMath::Pow((Start.Y - target.Y), 2) + FMath::Pow((Start.Z - target.Z), 2));

	return dis;
}
///chases a low health player
void ACore1::Chasemode(float DeltaTime)
{
	FVector targetloc = Player->GetActorLocation();
	FVector Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(targetloc - Currentloc).Rotator();
	this->SetActorRotation(Rot);

	const FVector Movementloc = FVector(150.f * DeltaTime , 0.f , 0.f);
	AddActorLocalOffset(Movementloc, true);

	ACore1::firebullet();
}