// Fill out your copyright notice in the Description page of Project Settings.

#include "OvertaleHacker.h"
#include "Playercharacter.h"

#include "PCBullet.h"

/**
* ... Player character ...
*/

/// Sets default values
APlayercharacter::APlayercharacter()
{
	/// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Playermeshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Playermesh"));
	RootComponent = Playermeshcomp;
	Playermeshcomp->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		Playermesh(TEXT("StaticMesh'/Game/Overtale/Models/Player.Player'"));
	if (Playermesh.Succeeded()) {
		Playermeshcomp->SetStaticMesh(Playermesh.Object);
	}

	Currentforwardspeed = 0.0f;
	firespeed = 0.5f;
	bulletoffset = 90.f;
	abletofire = true;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		laseraudio(TEXT("SoundWave'/Game/Overtale/Music/laser_shots.laser_shots'"));
	if (laseraudio.Succeeded())
	{
		lasersound = laseraudio.Object;
	}
}

/// Called when the game starts or when spawned
void APlayercharacter::BeginPlay()
{
	Super::BeginPlay();

	GEngine->GameViewport->Viewport->LockMouseToViewport(true);
}

/// Called every frame
void APlayercharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Movementloc = FVector(Currentforwardspeed * DeltaTime, Currentrightspeed * DeltaTime, 0.f);
	AddActorLocalOffset(Movementloc, true);

	Facemouse();

	if (currentfire != 0.f)
	{
		firing();
	}
}

/// Called to bind functionality to input
void APlayercharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("Moveforward", this, &APlayercharacter::MoveForwards);
	InputComponent->BindAxis("Moveright", this, &APlayercharacter::MoveRight);
	InputComponent->BindAxis("Fire", this, &APlayercharacter::firebutton);
}
///move forwards or backwards called on button press
void APlayercharacter::MoveForwards(float val)
{
	bool buttonpresseed = !FMath::IsNearlyEqual(val, 0.f);

	if (buttonpresseed)
	{
		Currentforwardspeed = val * 550.0f;
	}
	else
	{
		Currentforwardspeed = 0.0f;
	}
}
///move right or left called on button press
void APlayercharacter::MoveRight(float val)
{
	bool buttonpresseed = !FMath::IsNearlyEqual(val, 0.f);

	if (buttonpresseed)
	{
		Currentrightspeed = val * 550.0f;
	}
	else
	{
		Currentrightspeed = 0.0f;
	}
}
///sets the fire val to 1
void APlayercharacter::firebutton(float val)
{

	currentfire = val;

}
///makes the player face the mouse
void APlayercharacter::Facemouse()
{
	if (Controller)
	{
		FVector mouseLocation, mouseDirection;
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		PlayerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		FVector target(mouseLocation.X * 145, mouseLocation.Y * 145, this->GetActorLocation().Z);
		FRotator currentRotation = this->GetActorRotation();
		FRotator newRotation = FRotationMatrix::MakeFromX(target - this->GetActorLocation()).Rotator();

		FRotator applyRotation = FRotator(currentRotation.Pitch, newRotation.Yaw, currentRotation.Roll);
		this->SetActorRotation(applyRotation);
	}
}
///spawns a bullet on a timer cooldown
void APlayercharacter::firing()
{
	if (abletofire)
	{
		FRotator Spawnrot = this->GetActorRotation();

		FVector Spawnloc = GetActorLocation();

		FVector ForwVector = GetActorForwardVector();

		ForwVector = ForwVector * bulletoffset;

		Spawnloc = Spawnloc + ForwVector;

		UWorld* const World = GetWorld();

		if (World != NULL)
		{
			APCBullet* NewActor = World->SpawnActor<APCBullet>(Spawnloc, Spawnrot);
			if (lasersound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, lasersound, GetActorLocation());
			}
		}
		World->GetTimerManager().SetTimer(Timebetweenshots, this, &APlayercharacter::Allowfire, firespeed);
		abletofire = false;
	}
}
///resets the fire cooldown
void APlayercharacter::Allowfire()
{
	abletofire = true;
}