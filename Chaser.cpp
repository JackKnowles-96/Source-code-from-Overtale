// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Chaser.h"
#include "PCBullet.h"
#include "PCstate.h"
#include "Playercharacter.h"

// Sets default values
AChaser::AChaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chasermeshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("chasermesh"));
	RootComponent = chasermeshcomp;
	chasermeshcomp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Chashermesh(TEXT("StaticMesh'/Game/Overtale/Models/chaser.chaser'"));
	if (Chashermesh.Succeeded()) {
		chasermeshcomp->SetStaticMesh(Chashermesh.Object);
	}

	chasermeshcomp->OnComponentHit.AddDynamic(this, &AChaser::Hitobject);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		Deathaudio(TEXT("SoundWave'/Game/Overtale/Music/small_blast.small_blast'"));
	if (Deathaudio.Succeeded())
	{
		Deathsound = Deathaudio.Object;
	}
}

// Called when the game starts or when spawned
void AChaser::BeginPlay()
{

	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	FVector scale(1.8f, 1.8f, 1.8f);

	this->SetActorScale3D(scale);

	State = Cast<APCstate>(Player->PlayerState);
}

// Called every frame
void AChaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	const FVector Movementloc = FVector(50 * DeltaTime, 0.f, 0.f);
	targetloc = Player->GetActorLocation();
	Currentloc = this->GetActorLocation();
	FRotator Rot = FRotationMatrix::MakeFromX(targetloc - Currentloc).Rotator();
	this->SetActorRotation(Rot);
	this->AddActorLocalOffset(Movementloc, true);
}

void AChaser::Hitobject(UPrimitiveComponent* Hitcom, AActor* OtherAct, UPrimitiveComponent* OtherCom, FVector Impulse, const FHitResult& Hit)
{
	if (OtherAct->GetClass()->IsChildOf(APCBullet::StaticClass()))
	{
		Destroy();
		OtherAct->Destroy();
		State->setscore(10);
		if (Deathsound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Deathsound, GetActorLocation());
		}
	}
	if (OtherAct->GetClass()->IsChildOf(APlayercharacter::StaticClass()))
	{
		State->Sethealth(5);
		Destroy();
		if (Deathsound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Deathsound, GetActorLocation());
		}
	}
	if (OtherAct->GetClass()->IsChildOf(AChaser::StaticClass()))
	{
		chasermeshcomp->MoveIgnoreActors.Add(OtherAct);
	}
}