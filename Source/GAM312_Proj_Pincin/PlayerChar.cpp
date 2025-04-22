// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));
	// creates the camera component

	PlayerCamComp->SetupAttachment(GetMesh(), "Head");
	// attaches camera to the player mesh's head

	PlayerCamComp->bUsePawnControlRotation = true;
	// allows the camera to rotate with the players head movement

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	// Allows the player to move forward and backwards
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	// Allows the player to move right and left
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	// Allows the player to look up and down with the mouse Y axis
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	// Allows the player to look left and right with the mouse X axis
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	// Allow player to jump with the space bar pressed
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
	// Forces stop jump when the space bar is released
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	// allows the player to interact with objects within the parameters set up for distance and viewport

}
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
	// allows moovement forward and backward
}

void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
	// allows movement left and right
}

void APlayerChar::StartJump()
{
	bPressedJump = true;
	// press to jump
}

void APlayerChar::StopJump()
{
	bPressedJump = false;
	// not pressing space means you won't jump
}

void APlayerChar::FindObject()
{
}
